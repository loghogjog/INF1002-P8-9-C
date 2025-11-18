# include "snapshot.h"

// PLACE SNAPSHOTS IN SEPERATE DIRECTORY
bool file_in_dir(const char *path, const char *snapshot_name) {
    // check if dir exists
    // creates dir if not
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0755);
    }

    // open dir and search for file
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);

    if (dir == NULL) {
        perror("Failed to open directory %s.\n");
        return false;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, snapshot_name) == 0)
            break;
    } 
    closedir(dir);

    if (entry == NULL) {
        printf("File %s not found in snapshots directory.\n", entry->d_name);
        return false;
    }
        
    return true;
}

// Validate snapshot name to prevent path traversal
bool is_valid_snapshot_name(const char *snapshot_name) {
    if (snapshot_name == NULL || strlen(snapshot_name) == 0) {
        return false;
    }

    // Check for path traversal attempts
    if (strstr(snapshot_name, "..") != NULL ||
        strchr(snapshot_name, '/') != NULL ||
        strchr(snapshot_name, '\\') != NULL) {
        printf("Invalid snapshot name. Cannot contain '..' or path separators.\n");
        return false;
    }
    
    // Optional: limit length to prevent buffer issues
    if (strlen(snapshot_name) > 255) {
        printf("Snapshot name too long.\n");
        return false;
    }
    
    return true;
}

// construct safe file path
bool construct_snapshot_path(const char *cwd, const char *snapshot_name, 
                             char *buffer, size_t buffer_size) {
    // Use snprintf for safe string concatenation
    int written = snprintf(buffer, buffer_size, "%s/snapshots/%s", cwd, snapshot_name);
    
    if (written < 0 || written >= buffer_size) {
        printf("Error: Path too long\n");
        return false;
    }
    
    return true;
}

// SNAPSHOT FUNCTION
bool create_snapshot(const char *snapshot_name, char *cwd) {
    // validate no funny business in snapshot name
    if (!is_valid_snapshot_name(snapshot_name)) {
        printf("Invalid snapshot name.\n");
        return false;
    }

    // create safe snapshots dir path
    char dir_path[PATH_MAX];
    if (snprintf(dir_path, sizeof(dir_path), "%s/snapshots", cwd) >= sizeof(dir_path)) {
        printf("Error: Directory path too long.\n");
        return false;
    }

    // validate snapshot folder exists and snapshot file not in it
    if (file_in_dir(dir_path, snapshot_name)) {
        printf("Snapshot \"%s\" not in snapshots folder.\n", snapshot_name);
        return false;
    }

    // construct full file path
    char file_path[PATH_MAX];
    if (!construct_snapshot_path(cwd, snapshot_name, file_path, sizeof(file_path))) {
        return false;
    }

    // Check if snapshot file already exists
    FILE *snapshot_file = fopen(file_path, "r");
    if (snapshot_file != NULL) {
        printf("Snapshot \"%s\" already exists. Use a different name. \n", snapshot_name);
        fclose(snapshot_file);
        return false;
    }

    // Check if DB file exists
    FILE *db_file = fopen(FILENAME, "r");
    if (db_file == NULL) {
        printf("Unable to open database file \"%s\"", FILENAME);
        return false;
    }
    
    struct Record *records;
    int records_size = open_and_read_file(&records, FILENAME);

    if (fclose(db_file) != 0) {
        printf("Error closing database file \"%s\"\n", FILENAME);
        free(records);
        return false;
    }

    if (records_size < 0) {
        printf("Unable to read from database file \"%s\"\n", FILENAME);
        free(records);
        return false;
    }

    snapshot_file = fopen(file_path, "w");
    if (snapshot_file == NULL) {
        printf("Unable to create snapshot file \"%s\"\n", snapshot_name);
        free(records);
        return false;
    }

    for (int i = 0; i < records_size; i++) {
        fprintf(snapshot_file, "%d,%s,%s,%.2f\n", records[i].id, records[i].name, records[i].prog, records[i].marks);
    }

    if (fclose(snapshot_file) != 0) {
        printf("Error closing snapshot file \"%s\"\n", snapshot_name);
        free(records);
        return false;
    }

    printf("Snapshot created. Verifying...\n");
    
    struct Record *snapshot;
    int snapshot_size = open_and_read_file(&snapshot, file_path);
    
    if (snapshot_size != records_size) {
        printf("Snapshot verification failed. Snapshot \"%s\" size does not match current record \"%s\" size.\n", snapshot_name, FILENAME);
        free(records);
        free(snapshot);
        return false;
    }

    printf("Snapshot \"%s\" successfully created.\n", snapshot_name);

    free(records);
    free(snapshot);

    return true;
}


// RESTORE FUNCTION
bool restore_snapshot(const char *snapshot_name, char *cwd) {
    // validate no funny business in snapshot name
    if (!is_valid_snapshot_name(snapshot_name)) {
        printf("Invalid snapshot name.\n");
        return false;
    }

    // create safe snapshots dir path
    char dir_path[PATH_MAX];
    if (snprintf(dir_path, sizeof(dir_path), "%s/snapshots", cwd) >= sizeof(dir_path)) {
        printf("Error: Directory path too long.\n");
        return false;
    }

    // validate snapshot folder exists and snapshot file in it
    if (!file_in_dir(dir_path, snapshot_name)) {
        printf("Snapshot \"%s\" not in snapshots folder.\n", snapshot_name);
        return false;
    }

    // construct full file path
    char file_path[PATH_MAX];
    if (!construct_snapshot_path(cwd, snapshot_name, file_path, sizeof(file_path))) {
        return false;
    }
    
    FILE *snapshot_file = fopen(file_path, "r");

    if (snapshot_file == NULL) {
        printf("Snapshot file \"%s\" does not exist.\n",snapshot_name);
        return false;
    }
    fclose(snapshot_file);

    struct Record *snapshot_records;
    int snapshot_size = open_and_read_file(&snapshot_records, file_path);

    if (snapshot_size < 0) {
        printf("Unable to read from snapshot file \"%s\".\n", snapshot_name);
        return false;
    }

    FILE *db_file = fopen(FILENAME, "w");

    if (db_file == NULL) {
        printf("Unable to open database file \"%s\" for writing.\n", FILENAME);
        free(snapshot_records);
        return false;
    }

    for (int i = 0; i < snapshot_size; i++) {
        fprintf(db_file, "%d,%s,%s,%.2f\n", snapshot_records[i].id, snapshot_records[i].name, snapshot_records[i].prog, snapshot_records[i].marks);
    }

    if (fclose(db_file) != 0) {
        printf("Error closing database file \"%s\" after restoring.\n", FILENAME);
        free(snapshot_records);
        return false;
    }
    printf("Snapshot restored. Verifying...\n");

    struct Record *restored_records;
    int restored_size = open_and_read_file(&restored_records, FILENAME);

    if (restored_size != snapshot_size) {
        printf("Restoration verification failed. Restored size does not match snapshot size.\n");
        free(snapshot_records);
        free(restored_records);
        return false;
    }

    printf("Snapshot \"%s\" successfully restored.\n", snapshot_name);

    free(snapshot_records);
    free(restored_records);

    return true; 
}

//TODO:TEST COMMAND INJECTION 
bool delete_snapshot(const char *snapshot_name, char *cwd) {
    if (!is_valid_snapshot_name(snapshot_name)) {
        printf("Invalid snapshot name provided.\n");
        return false;
    }
    
    char dir_path[PATH_MAX];
    if (snprintf(dir_path, sizeof(dir_path), "%s/snapshots", cwd) >= sizeof(dir_path)) {
        printf("Error: Directory path too long.\n");
        return false;
    }

    if (!file_in_dir(dir_path, snapshot_name)) {
        printf("Snapshot \"%s\" not in snapshots folder.\n", snapshot_name);
        return false;
    }

    // construct full file path
    char file_path[PATH_MAX];
    if (!construct_snapshot_path(cwd, snapshot_name, file_path, sizeof(file_path))) {
        return false;
    }

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Snapshot \"%s\" does not exist.\n", snapshot_name);
        return false;
    }
    fclose(file);
    

    if (remove(file_path) == 0) { // only deletes from snapshot folder
        printf("Snapshot \"%s\" successfully deleted.\n", snapshot_name);
    } else {
        printf("Unable to delete snapshot \"%s\".\n", snapshot_name);
    }
    return true;
}

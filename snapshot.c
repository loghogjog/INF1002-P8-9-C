# include "snapshot.h"

// SNAPSHOT FUNCTION
void create_snapshot(const char *snapshot_name) {
    // Check if snapshot file already exists
    FILE *snapshot_file = fopen(snapshot_name, "r");
    if (snapshot_file != NULL) {
        printf("Snapshot \"%s\" already exists. Use a different name. \n", snapshot_name);
        fclose(snapshot_file);
        return;
    }

    // Check if DB file exists
    FILE *db_file = fopen(FILENAME, "r");
    if (db_file == NULL) {
        printf("Unable to open database file \"%s\"", FILENAME);
        return;
    }
    
    struct Record *records;
    int records_size = open_and_read_file(&records, FILENAME);

    if (fclose(db_file) != 0) {
        printf("Error closing database file \"%s\"\n", FILENAME);
        free(records);
        return;
    }

    if (records_size < 0) {
        printf("Unable to read from database file \"%s\"\n", FILENAME);
        free(records);
        return;
    }

    snapshot_file = fopen(snapshot_name, "w");
    if (snapshot_file == NULL) {
        printf("Unable to create snapshot file \"%s\"\n", snapshot_name);
        free(records);
        return;
    }

    for (int i = 0; i < records_size; i++) {
        fprintf(snapshot_file, "%d,%s,%s,%.2f\n", records[i].id, records[i].name, records[i].prog, records[i].marks);
    }

    printf("Snapshot created. Verifying...\n");
    
    struct Record *snapshot;
    int snapshot_size = open_and_read_file(&snapshot, snapshot_name);
    
    if (snapshot_size != records_size) {
        printf("Snapshot verification failed. Snapshot size does not match current record size.\n");
        free(records);
        free(snapshot);
        return;
    }

    if (fclose(snapshot_file) != 0) {
        printf("Error closing snapshot file \"%s\"\n", snapshot_name);
        free(records);
        free(snapshot);
        return;
    }

    printf("Snapshot \"%s\" successfully created.\n", snapshot_name);

    free(records);
    free(snapshot);

    return;
}


// RESTORE FUNCTION
void restore_snapshot(const char *snapshot_name) {
    FILE *snapshot_file = fopen(snapshot_name, "r");

    if (snapshot_file == NULL) {
        printf("Snapshot file \"%s\" does not exist.\n",snapshot_name);
        return;
    }
    fclose(snapshot_file);

    struct Record *snapshot_records;
    int snapshot_size = open_and_read_file(&snapshot_records, snapshot_name);

    if (snapshot_size < 0) {
        printf("Unable to read from snapshot file \"%s\".\n", snapshot_name);
        return;
    }

    FILE *db_file = fopen(FILENAME, "w");

    if (db_file == NULL) {
        printf("Unable to open database file \"%s\" for writing.\n", FILENAME);
        free(snapshot_records);
        return;
    }

    for (int i = 0; i < snapshot_size; i++) {
        fprintf(db_file, "%d,%s,%s,%.2f\n", snapshot_records[i].id, snapshot_records[i].name, snapshot_records[i].prog, snapshot_records[i].marks);
    }

    if (fclose(db_file) != 0) {
        printf("Error closing database file \"%s\" after restoring.\n", FILENAME);
        free(snapshot_records);
        return;
    }
    printf("Snapshot restored. Verifying...\n");

    struct Record *restored_records;
    int restored_size = open_and_read_file(&restored_records, FILENAME);

    if (restored_size != snapshot_size) {
        printf("Restoration verification failed. Restored size does not match snapshot size.\n");
        free(snapshot_records);
        free(restored_records);
        return;
    }

    printf("Snapshot \"%s\" successfully restored.\n", snapshot_name);

    free(snapshot_records);
    free(restored_records);

    return; 
}

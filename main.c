# include "headers.h"

int main(){
    /* === DECLARTION ===*/
    printf("%55s\n", "Declaration");
    printf("%s",
        "SIT’s policy on copying does not allow the students to copy source code as well as assessment solutions\n"
        "from another person AI or other places. It is the students’ responsibility to guarantee that their\n"
        "assessment solutions are their own work. Meanwhile, the students must also ensure that their work is\n"
        "not accessible by others. Where such plagiarism is detected, both of the assessments involved will\n"
        "receive ZERO mark.\n\n"
        "We hereby declare that:\n"
        "\u2022 We fully understand and agree to the abovementioned plagiarism policy.\n"
        "\u2022 We did not copy any code from others or from other places.\n"
        "\u2022 We did not share our codes with others or upload to any other places for public access and will not do that in the future.\n"
        "\u2022 We agree that our project will receive Zero mark if there is any plagiarism detected.\n"
        "\u2022 We agree that we will not disclose any information or material of the group project to others or upload to any other places for public access.\n"
        "\u2022 We agree that we did not copy any code directly from AI generated sources\n\n"
        "Declared by: INF1002 P8-9\n"
        "Team members:\n"
        "1. Tristan Tan\n"
        "2. Alvan Loh\n"
        "3. Tristan Koh\n"
        );
    printf("Date: %d\n\n", DATE_COMPLETED);

    /* === CMS START === */
    printf("%c", '*');
    for (int i = 0; i < 50; i++) {
        printf("%c", '-');
    }
    printf("%s", "Welcome to P8-9 CMS");
    for (int i = 0; i < 50; i++) {
        printf("%c", '-');
    }
    printf("%c\n", '*');

    /* === MAIN FUNCTION === */

    bool file_opened = false; // check if DB opened
    struct Record *records = NULL;
    int records_size = 0;

    do {
        // Get User Input
        char input[MAX_INPUT] = "";
        bool success = false;

        do { // explore using strtok directly

            if (records && records_size > 0) {
                for (size_t i = 0; i < records_size; i++) {
                    printf("ID: %d, Name: %s, Prog: %s, Marks: %.2f\n", records[i].id, records[i].name, records[i].prog, records[i].marks);
                }
            }

            printf("%s: ", USER);
            if (fgets(input, sizeof(input), stdin) == NULL) {
                // EOF or error
                printf("\nExiting...\n");
                if (records) free(records);
                return 0;
            }

            printf("%s: ", CMS);

            // buffer overflow
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] != '\n') { // check if last char is \n
                int c;
                while ((c = getchar()) != '\n' && c != EOF); // clear buffer

                printf("Input too long, max %d characters\n", MAX_INPUT - 2); // <-- fgets + \n
                continue;
            }

            if (len <= 1) {
                printf("Enter a command.\n");
                continue;
            }
            // insert more validation below

            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
                len--;
            }

            success = true;
        } while (!success);

        to_lower(input);
        char *input_copy = strdup(input); // tokenize copy to preserve original input jic

        if (input_copy == NULL) {
            if (input_copy == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                if (records) free(records);
                return 1;
            }
        }

        char *token = strtok(input_copy, " ");
        if (token == NULL) {
            free(input_copy);
            continue;
        }

        // Exit
        if (strcmp(input, "exit") == 0) {
            printf("Exiting...Goodbye :)\n");
            // TO FREE MEMORY USED BY ARRAY
            if (records_size > 0) {
                free(records);
            }
            return 0;
        }
        // help menu
        else if (strcmp(token, "help") == 0) {
            char *arg = strtok(NULL, " ");
            help_menu(arg);
            free(input_copy);
            continue;
        }
        /* OPEN FILE (TRISTAN) */
        else if (strcmp(token, "open") == 0) {
            // flush previous values if any
            if (records) {
                free(records);
                records = NULL;
                records_size = 0;
            }
            // call open file func
            records_size = open_and_read_file(&records, FILENAME);

            if (records == NULL || records_size < 0) {
                printf("Failed to read from database file %s.\n", FILENAME);
                free(input_copy);
                continue;
            }

            printf("The database file \"%s\" is successfully opened.\n", FILENAME);
            file_opened = true;
            free(input_copy);
            continue;
        }
        // UNIQUE (SNAPSHOT)
        else if (strstr(input, "snapshot") != NULL) {
            char cwd[512] = "";
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                perror("Error getting current working directory");
                free(input_copy);
                continue;
            }
            
            // Create snapshots directory path
            char dir_path[PATH_MAX] = "";
            if (snprintf(dir_path, sizeof(dir_path), "%s/snapshots", cwd) >= sizeof(dir_path)) {
                printf("Error: Directory path too long.\n");
                free(input_copy);
                continue;
            }
            
            char *snapshot_ptr = NULL;
            char *snapshot_name = NULL;
            
            // SHOW SNAPSHOT - list all snapshots
            if ((snapshot_ptr = strstr(input, "show snapshot")) != NULL) {
                printf("\nAvailable snapshots:\n");
                file_in_dir(dir_path, NULL);  // Pass NULL to list all files
                free(input_copy);
                continue;
            }
            // CREATE SNAPSHOT
            else if ((snapshot_ptr = strstr(input, "create snapshot")) != NULL) {
                snapshot_name = snapshot_ptr + strlen("create snapshot ");
                
                // Trim leading spaces
                while (*snapshot_name == ' ') snapshot_name++;
                
                if (strlen(snapshot_name) == 0) {
                    printf("Snapshot name not provided.\n");
                    free(input_copy);
                    continue;
                }
                if (strlen(snapshot_name) > 50) {
                    printf("Snapshot name too long. Max 50 characters.\n");
                    free(input_copy);
                    continue;
                }
                
                // construct the file path with the actual snapshot_name
                char file_path[PATH_MAX] = "";
                if (!construct_snapshot_path(cwd, snapshot_name, file_path, sizeof(file_path))) {
                    free(input_copy);
                    continue;
                }
                
                create_snapshot(snapshot_name, file_path);
                free(input_copy);
                continue;
            }
            // RESTORE SNAPSHOT
            else if ((snapshot_ptr = strstr(input, "restore snapshot")) != NULL) {
                snapshot_name = snapshot_ptr + strlen("restore snapshot ");
                
                while (*snapshot_name == ' ') snapshot_name++;
                
                if (strlen(snapshot_name) == 0) {
                    printf("Snapshot name not provided.\n");
                    free(input_copy);
                    continue;
                }
                
                char file_path[PATH_MAX] = "";
                if (!construct_snapshot_path(cwd, snapshot_name, file_path, sizeof(file_path))) {
                    free(input_copy);
                    continue;
                }
                
                restore_snapshot(snapshot_name, file_path);
                free(input_copy);
                continue;
            }
            // DELETE SNAPSHOT
            else if ((snapshot_ptr = strstr(input, "delete snapshot")) != NULL) {
                snapshot_name = snapshot_ptr + strlen("delete snapshot ");
                
                while (*snapshot_name == ' ') snapshot_name++;
                
                if (strlen(snapshot_name) == 0) {
                    printf("Snapshot name not provided.\n");
                    free(input_copy);
                    continue;
                }
                
                char file_path[PATH_MAX] = "";
                if (!construct_snapshot_path(cwd, snapshot_name, file_path, sizeof(file_path))) {
                    free(input_copy);
                    continue;
                }
                
                delete_snapshot(snapshot_name, file_path);
                free(input_copy);
                continue;
            }
            else {
                printf("Use HELP SNAPSHOT to open help menu.\n");
                free(input_copy);
                continue;
            }
        }
        /*
         * READ THIS:
         * Records is an array of Structures (Classes)
         * Loop through the array and access attributes of each structure (row) using:
         * records[i].<element> <-- refer to open.h for attribute names
         * use records_size for your loop sizeof
         */
        // DO OPERATIONS ON RECORDS
        else if (!file_opened) {
            printf("Open database file first.\n");
            free(input_copy);
            continue;
        }
        // INSERT
        if (strcmp(token, "insert") == 0)  {
            //check for entry first
            bool ins_fail = false;
            struct Record new_record;
            ins_fail = insert(new_record, records, &records_size, token); // key function
            if (ins_fail) {
                continue; // error msgs printed in insert()
            }
        }
        // QUERY
        // DELETE
        // SORTING
        // UPDATE

        // SAVE
        else if (strcmp(token, "save") == 0) {
            save(records, records_size);
        }
        // SHOW ALL
        else if (strcmp(token, "showall") == 0) {
            showall(records, records_size);
        }
        // SUMMARY
        else if (strcmp(token, "showsummary") == 0) {
            showsummary(records, records_size);
        }
        printf("Unknown command.\n");
        free(input_copy);


    } while (true);


    return 0;
}

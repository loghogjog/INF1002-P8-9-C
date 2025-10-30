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
    do {
        // Get User Input
        char input[MAX_INPUT];
        bool success = false; 
        
        do { // explore using strtok directly

            printf("%s: ", USER);

            fgets(input, sizeof(input), stdin);
            //input[strcspn(input, "\n")] = 0;

            printf("%s: ", CMS);

            // buffer overflow
            int len = strlen(input);
            if (len > 0 && input[len - 1] != '\n') { // check if last char is \n
                int c;
                while ((c = getchar()) != '\n' && c != EOF); // clear buffer
                
                printf("Input too long, max %d characters\n", MAX_INPUT - 2); // <-- fgets + \n
                continue;
            }

            if (len == 1) {
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
        
        to_upper(input);       
        struct Record *records;
        int records_size;
        char *token = strtok(input, " ");

        /* OPEN FILE (TRISTAN) */
        if (strcmp(token, "OPEN") == 0) {
            // call open file func
            records_size = open_and_read_file(&records);

            if (!records) {
                printf("Failed to read from database file %s.\n", FILENAME);
                return 1;
            }
            if (records_size <= 0) {
                printf("Invalid record size.\n");
                return 1;
            }
            // -- more validation here -- // 

            printf("The database file \"%s\" is successfully opened.\n", FILENAME);
            file_opened = true;

            
        }
        /*
         * READ THIS:
         * Records is an array of Structures (Classes)
         * Loop through the array and access attributes of each structure (row) using:
         * records[i].<element> <-- refer to open.h for attribute names
         * use records_size for your loop sizeof
         */
        // DO OPERATIONS ON RECORDS 

        // SHOW ALL (ALVAN)
        
        // SAVE
        
        // INSERT
        else if ((strcmp(token, "INSERT") == 0) && file_opened) {
            //check for entry first
            char *attributes[] = {"ID", "NAME", "PROG", "MARK"};
            bool ins_fail = false;
            InsertData data;

            for (int i = 0; i < 4; i++){
                token = strtok(NULL, INSERT_DELIM); // "=" === key

                if (token == NULL) {
                    if (i == 0) {
                        printf("Provide ID field.\n");
                        ins_fail = true;
                        break;
                    }
                }
                
                // token not null
                to_upper(token);
                char *key = token; // store key (temp var)

                // check if value in input (reverse checking)
                if ((token = strtok(NULL, " ")) != NULL) {
                    if (strcmp(key, "ID") == 0) {
                        printf("%s\n", token);
                        for (int i = 0; i < ((sizeof(token) / sizeof(token[0])) - 1); i++) {
                            if (!isdigit(token[i])) {
                               printf("%c %d\n", token[i], i); 
                                printf("ID should only have integers.\n");
                                ins_fail = true;
                                break;
                            }
                        }
                        data.id = atoi(token);
                        data.has_id = true;
                    }
                    else if (strcmp(key, "NAME") == 0) {
                        if (strlen(token) > 100) {
                            printf("Name max 100 characters.\n");
                            ins_fail = true;
                            break;
                        }
                        strncpy(data.name, token, 99);
                        data.name[99] = 0;
                        data.has_name = true;
                    }
                    else if (strcmp(key, "PROG") == 0) {
                        if (strlen(token) > 250) {
                            printf("Programme max 250 characters.\n");
                            ins_fail = true;
                            break;
                        }
                        strncpy(data.prog, token, 249);
                        data.prog[249] = 0; 
                        data.has_prog = true;
                    }
                    else if (strcmp(key, "MARK") == 0) {
                        float value;
                        char *endptr;

                        value = strtof(token, &endptr);
                        if (*endptr == "\0" || endptr == token) {
                            printf("Marks should be float value.\n");
                            ins_fail = true;
                            break;
                        }
                        data.mark = value;
                        data.has_mark = true;
                    }
                }
                else {
                    if (!data.has_id) {
                        printf("Provide ID field\n");
                        ins_fail = true;
                        break;
                    }
                }
            }

            if (ins_fail) continue; 

            if (data.has_id) {
                int user_exist = check_record_exists(data.id, records_size, records);

                if (user_exist == 1) { // user found
                    printf("The record with ID=%d already exists.\n", data.id);
                    ins_fail = true;
                }

                if (!data.has_name || !data.has_prog || !data.has_mark) {
                    printf("Must have Name, Programme, and Mark fields.\n");
                }

                int insert_result = insert(data, records);

                if (insert_result != 0) {
                    printf("Error adding record with ID=%d\n", data.id);
                    ins_fail = true;
                }
                else {
                    printf("A new record with ID=%d has been added.\n", data.id);
                }
            } 
                            
            if (ins_fail) continue; 
            // for (int i = 0; i < (sizeof(attributes) / sizeof(attributes[0])); i++) {
            //
            //     if (i == 0 && token == NULL) {
            //         printf("Enter a valid INSERT input.\n");
            //         ins_fail = true;
            //         break;
            //     }
            //     else if (token == NULL) {
            //         continue;
            //     }
            //     to_upper(token);
            //     if (strcmp(token, attributes[i]) != 0) {
            //         printf("Please enter %s Field.\n", attributes[i]);
            //         ins_fail = true;
            //         continue;
            //     }
            //     token = strtok(NULL, " "); // corresponding value
            //     if (token != NULL) {
            //         if (i == 0) {
            //             unsigned int id = atoi(token);
            //             // if id in records
            //             if ((check_record_exists(id, records_size, records)) == 1) {
            //                 printf("The record with ID=%d already exists.\n", id);
            //                 continue;
            //             }
            //             else {
            //
            //             }
            //         }
            //     }
            //     else{
            //         ins_fail = true;
            //         break;
            //     }
            //
            // }

        }
        //insert new entry

        // QUERY
        
        // UPDATE
        
        // DELETE
        
        
        // SORTING
        
        // SUMMARY (ALVAN)
        
        // UNIQUE
        else if (!file_opened) {
            printf("Open database file first.\n");
            continue;
        }
        for (size_t i = 0; i < records_size; i++) {
            printf("ID: %d, Name: %s, Prog: %s, Marks: %.2f\n", records[i].id, records[i].name, records[i].prog, records[i].marks); 
        }
    } while (true);
       return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include "headers.h"

// define as global variables
struct Record *records = NULL;
int records_size = 0;

int main(){
    /* === CMS START === */
    printf("%c", '*');
    for (int i = 0; i < 50; i++) {
        printf("%c", '-');
    }
    printf("%s", "Welcome to the CMS");
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
             
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
                len--;
            }

            success = true;
        } while (!success);
        
        to_lower(input);       
        char *input_copy = strdup(input); // to preserve original input in case
        printf("%s\n", input_copy); // debug
        
        if (input_copy == NULL) {
            printf("Input duplicate failed.\n");
            return 1;
        }

        // moved out of main 
        char *token = strtok(input_copy, " ");

        /* OPEN FILE */
        if (strcmp(token, "open") == 0) {
            // call open file func
            records_size = open_and_read_file(&records, FILENAME);

            if (!records) {
                printf("Failed to read from database file %s.\n", FILENAME);
                return 1;
            }
            if (records_size <= 0) {
                printf("Invalid record size.\n");
                return 1;
            }

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
        // TODO: CREATE HELP MENU
        // CAN DISPLAY BEFORE OR AFTER WRONG COMMAND ENTERED (TBD)

        // SHOW ALL (ALVAN)
        
        // SAVE (ALVAN)
        
        // INSERT
        else if ((strcmp(token, "insert") == 0) && file_opened) {
            //check for entry first
            bool ins_fail = false;
            struct Record new_record = { 0 };
            ins_fail = insert(new_record, records, &records_size, token); // key function
            if (ins_fail) {
                continue; // error msgs printed in insert()
            }

        }

        // QUERY (TRISTAN KOH)
        else if ((strcmp(token, "query") == 0) && file_opened) {
            char* args = strtok(NULL, "");
            if (args != NULL) {
                while (*args == ' ') args++;
                query(records, records_size, args);
            }
            else {
                printf("CMS: Invalid command. Follow the format: QUERY ID=<ID>\n");
            }
        }
        
        // UPDATE (TRISTAN KOH)
        else if ((strcmp(token, "update") == 0) && file_opened) {
            char* args = strtok(NULL, "");  // Get the rest of the line after "update"
            if (args != NULL) {
                // Skip leading spaces
                while (*args == ' ') args++;
                updateRecord(records, records_size, args);
            }
            else {
                printf("CMS: Invalid command format. Follow the format: UPDATE ID=<id> <Field>=<Value>\n\n");
            }
        }
        
        // DELETE (TRISTAN KOH)
        else if ((strcmp(token, "delete") == 0) && file_opened) {
            char* args = strtok(NULL, "");
            if (args != NULL) {
                while (*args == ' ') args++;
                delete_record(records, &records_size, args);
            }
            else {
                printf("CMS: Invalid command. Follow the format: DELETE ID=<ID>\n");
            }
        }

        // SORT (TRISTAN KOH)
        else if ((strcmp(token, "sort") == 0) && file_opened) {

            char* by = strtok(NULL, " ");
            char* field = strtok(NULL, " ");
            char* order = strtok(NULL, " ");

            if (!by || !field || !order || strcmp(by, "by") != 0) {
                printf("Please follow the format: SORT BY (ID / MARKS) (ASC / DESC)\n");
                continue;
            }

            sort_records(records, records_size, field, order);
        }

        
        // SUMMARY(ALVAN)
        
        
        // Exit 
        else if (strcmp(input, "exit") == 0) {
            printf("Exiting...Goodbye :)\n");
            break;
        }
        else if (!file_opened) {
            printf("Open the database file first with 'open'.\n");
            continue;
        }
        else {
            printf("Unknown command. Only the following commands are available: insert, query, update, delete, save, show all, exit\n");
            continue;
        }
        for (size_t i = 0; i < records_size; i++) {
            printf("ID: %d, Name: %s, Prog: %s, Marks: %.2f\n", records[i].id, records[i].name, records[i].prog, records[i].marks); 
        }
    } while (true);

    /* TO FREE MEMORY USED BY ARRAY
    for (size_t i = 0; i < total_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    */


    return 0;
}

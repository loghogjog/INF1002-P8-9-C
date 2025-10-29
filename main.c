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
    do {
        // Get User Input
        char input[MAX_INPUT];
        bool success = false; 
        
        do { // validation

            printf("%s: ", USER);
            
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("%s\n", "Invalid input.");
                continue;
            }

            // buffer overflow
            int len = strlen(input);
            if (len > 0 && input[len - 1] != '\n') { // check if last char is \n
                int c;
                while ((c = getchar()) != '\n' && c != EOF); // clear buffer
                
                printf("Input too long, max %d characters\n", MAX_INPUT - 2); // <-- fgets + \n
                continue;
            }

            // more validation below
             
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
                len--;
            }

            success = true;
        } while (!success);
        
        // convert to uppercase
        for (size_t i = 0; i < (sizeof(input) / sizeof(input[0])); i++){
            input[i] = toupper((unsigned char) input[i]);
        }
        

        /* OPEN FILE (TRISTAN) */
        if (strcmp(input, "OPEN") == 0) {
            struct Record *records = NULL;
            int records_size = open_and_read_file(&records);
            
            printf("CMS: \n");

            if (!records) {
                printf("Failed to read from database file %s.\n", FILENAME);
                return 1;
            }
            if (records_size <= 0) {
                printf("Invalid record size.\n");
            }
            // printf("%d", records_size)
            // for (size_t i = 0; i < records_size; i++) {
            //     printf("ID: %d, Name: %s, Prog: %s, Marks: %f\n", records[i].id, records[i].name, records[i].prog, records[i].marks); 
            // }
        }
        // READ THIS:
        // Records is an array of Structures (Classes)
        // Loop through the array and access attributes of each structure (row) using:
        // records[i].<element> <-- refer to open.h for attribute names
        // use records_size for your loop size
         
       
        // DO OPERATIONS ON FILE_CONTENT

        // SHOW ALL (ALVAN)
        
        // INSERT
        
        // QUERY
        
        // UPDATE
        
        // DELETE
        
        // SAVE
        
        // SORTING
        
        // SUMMARY (ALVAN)
        
        // UNIQUE

    } while (true);
       return 0;
}

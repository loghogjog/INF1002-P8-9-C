# include <stdio.h>
# include "constants.h"

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
    
    /* OPEN FILE (TRISTAN) */
    // * indicates variable is a pointer
    FILE *fp; 
    
    printf("Opening file...\n");
    fp = fopen(FILENAME, "r");
    
    // check if file opened successfully
    if (fp == NULL) {
        perror("Error opening file"); 
        return 1;
    }
    printf("File opened sucessfully!\n");
    
    // read file and store in buffer
    

    fclose(fp);

         
    // SHOW ALL (ALVAN)
    
    // INSERT
    
    // QUERY
    
    // UPDATE
    
    // DELETE
    
    // SAVE
    
    // SORTING
    
    // SUMMARY (ALVAN)
    
    // UNIQUE
    return 0;
}

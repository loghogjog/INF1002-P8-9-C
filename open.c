// Done by: Tristan Tan
# include "open.h"

char **open_and_read_file(){
    printf("Opening file...\n");
    FILE *file = fopen(FILENAME, "r");
    
    //check if file opened successfully
    if (file == NULL) {
        perror("Error opening file"); 
        return NULL;
    }
    printf("File opened sucessfully!\n");
    
    // read file and store in buffer
    char **lines; // <-- pointer to a pointer
    lines = malloc(sizeof(char *) * MORE_LINES);

    size_t total_lines = 0; //<-- size_t stores large non negative int
    size_t total_chars = 0;

    do {
        char c = fgetc(file);

        // error reading from file
        if (ferror(file)) {
            perror("Error reading from file");
            return NULL;
        }

        // reached EOF
        if (feof(file)) {
            // reallocating memory incase file does not end with \n or \0 
            if (total_chars != 0){
                lines[total_lines] = realloc(lines[total_lines], total_chars + 1); // <-- leave space for null byte
                lines[total_lines][total_chars] = '\0'; // <-- append terminating null byte
                
                total_lines++;
            }
            break;
        }

        if (total_chars == 0) {
            lines[total_lines] = malloc(MORE_CHARS); // <-- pointer created in lines[total_lines]
        }

        lines[total_lines][total_chars] = c; // <-- store character
        total_chars++;

        // reached EOL
        if (c == '\n') {
            // dynamically reallocate memory based on total no. of chars 
            lines[total_lines] = realloc(lines[total_lines], total_chars + 1); // <-- leave space for null byte
            lines[total_lines][total_chars] = '\0'; // <-- append terminating null byte
            
            total_lines++;
            total_chars = 0;

            // reached end of allocated lines
            if (total_lines % MORE_LINES == 0){
                size_t new_size = total_lines + MORE_LINES; // <-- new array length
                lines = realloc(lines, sizeof(char *) * new_size); // <-- sizeof as size of pointer may differ between compilers
            } 
        }
        // not enough space to store characters in current line
        else if (total_chars % MORE_CHARS == 0){
            size_t new_size = total_chars + MORE_CHARS;
            lines[total_lines] = realloc(lines[total_lines], new_size);
        }

    } while (true);

    // finally, reallocate array memory
    lines = realloc(lines, sizeof(char *) * total_lines);

    /* for testing
    for (size_t i = 0; i < total_lines; i++) {
        print("%s", lines[i]);
    }
    */

    /* TO FREE MEMORY USED BY LINES ARRAY
    for (size_t i = 0; i < total_lines; i++) {
        free(lines[i]);
    }
    free(lines);
    */

    fclose(file);

    return lines;
}

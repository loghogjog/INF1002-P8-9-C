// general miscellaneous functions
# include "misc.h"

void to_lower(char string[]){
    for (size_t i = 0; i < strlen(string); i++){
            string[i] = tolower((unsigned char) string[i]);
        }
}

int comp(const void *a, const void *b) {
    int *row1 = (int*)a;
    int *row2 = (int*)b;
    return (*row1 - *row2);
}

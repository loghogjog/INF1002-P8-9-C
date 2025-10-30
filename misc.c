#include "headers.h"

void to_upper(char string[]){
    for (size_t i = 0; i < strlen(string); i++){
            string[i] = toupper((unsigned char) string[i]);
        }
}
    

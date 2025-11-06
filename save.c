#include "headers.h"

int save(const char *filename, const char *data) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }
    fprintf(file, "%s", data);
    fclose(file);
    return 0;
}

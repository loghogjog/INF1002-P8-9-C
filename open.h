// Done by: Tristan Tan
#ifndef OPEN_H
#define OPEN_H

#define ARRAY_SIZE 1024 
#define OPEN_DELIM ","

#include "headers.h"

struct Record {
    int id;
    char name[100];
    char prog[256];
    float marks;
};

int open_and_read_file(struct Record **records);

#endif

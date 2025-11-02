// Done By: Tristan Tan
#ifndef INSERT_H
#define INSERT_H

#define INSERT_DELIM "="

// typedef struct {
//     unsigned int id;
//     char name[100];
//     char prog[200];
//     float mark;
//
//     bool has_id;
//     bool has_name;
//     bool has_prog;
//     bool has_mark;
// }InsertData;

int check_record_exists(int id, int *size, struct Record *records);

bool insert(struct Record data, struct Record *records, int *records_size, char *token);

#endif // !INSERT_H

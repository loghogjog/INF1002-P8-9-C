#include "headers.h"

int check_record_exists(int id, int size, struct Record *records) {
    for (int i = 0; i < size; i++) {
        if (records[i].id == id) {
            return 1;
        }
    }
    return 0;
}


int insert(InsertData data, struct Record *records) {
    return 0;
}

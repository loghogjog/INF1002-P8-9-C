#include "headers.h"

int showall(struct Record *records, int total_records)
{
    // header
    printf("\n");
    printf("All Records: \n");
    printf("\n");

    // check if anything in record file
    if (total_records == NULL)
    {
        printf("Error, No Records Found! \n");
        return -1;
    }

    // print table header
    printf("%-5s %-25s %-25s %-10s\n", "ID", "Name", "Program,", "Marks");
    printf("----------------------------------------------------------\n");

    // arr record in table
    int i;
    for (i = 0; i < total_records; i++)
    {
        printf("%-5d %-25s %-25s %-10.2f\n", records[i].id, records[i].name, records[i].prog, records[i].marks);
    }

    // footer
    printf("----------------------------------------------------------\n");

    // tabulate record number for easier viewing
    printf("Total Records: %d\n", total_records);

    return 0;
}

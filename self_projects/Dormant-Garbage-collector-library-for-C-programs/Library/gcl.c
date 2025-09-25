#include <stdio.h>
#include <stdlib.h>
#include "gcl.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char *DATA_TYPE[] = {"UINT8", "UINT32", "INT32",
                     "CHAR", "OBJ_PTR", "FLOAT",
                     "DOUBLE", "OBJ_STRUCT"};

void print_structure_rec(struct_db_rec_t *struct_rec)
{
    if(!struct_rec)
    {
        return;
    }

    int j = 0;
    field_info_t *field = NULL;

    printf(ANSI_COLOR_YELLOW "|------------------------------------------------------|\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN "| %-20s | size = %-8d | #flds = %-3d |\n" ANSI_COLOR_RESET, struct_rec->struct_name, struct_rec->ds_size, struct_rec->n_fields);
    printf(ANSI_COLOR_YELLOW "|------------------------------------------------------|------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    for(j = 0; j < struct_rec->n_fields; j++)
    {
        field = &struct_rec->fields[j];

        printf("  %-20s |", "");
        printf(ANSI_COLOR_RED "%-3d %-20s | dtype = %-15s | size = %-5d | offset = %-6d|  nstructname = %-20s  |\n" ANSI_COLOR_RESET,
                j, field->field_name, DATA_TYPE[field->data_type], field->size, field->offset, field->nested_struct_name);
        printf("  %-20s |", "");
        printf(ANSI_COLOR_YELLOW "--------------------------------------------------------------------------------------------------------------------------|\n" ANSI_COLOR_RESET);
    }
}

void print_structure_db(struct_db_t *struct_db)
{
    if(!struct_db)
    {
        return;
    }

    printf("Structure data base\n");

    int i = 0;
    struct_db_rec_t *struct_rec = NULL;
    struct_rec = struct_db->head;

    printf("No. of Structures registered = %d\n", struct_db->count);

    while(struct_rec)
    {
        printf("Structure no. : %d (%p)\n", i++, struct_rec);
        print_structure_rec(struct_rec);
        struct_rec = struct_rec->next;
    }
}

int add_structure_to_struct_db(struct_db_t *struct_db,
                           struct_db_rec_t *struct_rec)
{
    struct_db_rec_t *head = struct_db->head;

    if(!head)
    {
        struct_db->head = struct_rec;
        struct_rec->next = NULL;
        struct_db->count++;

        return 0;
    }

    struct_rec->next = head;
    struct_db->head = struct_rec;
    struct_db->count++;

    return 0;
}

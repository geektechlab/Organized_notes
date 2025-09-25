#ifndef __GCL_H
#define __GCL_H

#include <assert.h>

/*  Supporting macros */
#define MAXIMUM_STRUCT_NAME_SIZE 128
#define MAXIMUM_FIELD_NAME_SIZE  128

/* Compute offset of field within structure */
#define FLD_OFFSET_STRUCT(struct_name, fld_name)     \
    (unsigned int)&(((struct_name *)0)->fld_name)

/* Compute size of field within structure */
#define FIELD_SIZE_STRUCT(struct_name, fld_name)   \
    sizeof(((struct_name *)0)->fld_name)

/* Fetch field information */
#define FIELD_INFO(struct_name, fld_name, dtype, nested_struct_name)    \
   {#fld_name, dtype, FIELD_SIZE_STRUCT(struct_name, fld_name),                \
        FLD_OFFSET_STRUCT(struct_name, fld_name), #nested_struct_name}

/* Enum for application data types*/
typedef enum
{
    UINT8,
    UINT32,
    INT32,
    CHAR,
    OBJECT_PTR,
    FLOAT,
    DOUBLE,
    OBJECT_STRUCT
}data_type_t;

/* Structure to hold info about field within a structure*/
typedef struct _field_info_
{
    char field_name[MAXIMUM_FIELD_NAME_SIZE]; /* Name of the field */
    data_type_t data_type;                    /* Data type of the field */
    unsigned int size;                        /* Size of the field */
    unsigned int offset;                      /* Offset of the field */
    /* if data_type = OBJECT_PTR, Or OBJECT_STRUCT, then it holds details of that */
    /* Not useful if data_type is native */
    char nested_struct_name[MAXIMUM_STRUCT_NAME_SIZE];
}field_info_t;


/* This typedef is required for providing structure pointer data type in structure,
why only this ? */
typedef struct _struct_db_rec_ struct_db_rec_t;

/* Structure to hold info about application data types */
struct _struct_db_rec_
{
    /* /* Pointer to the next structure to create a linked list */
    struct_db_rec_t *next;
    char struct_name [MAXIMUM_STRUCT_NAME_SIZE]; /* Name of structure */
    unsigned int ds_size;                        /* Size of the structure */
    unsigned int n_fields;                       /* No. of fields within the structure*/
    field_info_t *fields;                        /* pointer to a structure holding fields' details */
};

/* Store the head of the structure linked list representing the structure database*/
typedef struct _struct_db_
{
    struct_db_rec_t *head;
    unsigned int count;
}struct_db_t;

/* Print particular structure records */
void print_structure_rec(struct_db_rec_t *struct_rec);

/* Print all structure records */
void print_structure_db(struct_db_t *struct_db);

/* Add a structure record in a structure database */
int add_structure_to_struct_db(struct_db_t *struct_db, struct_db_rec_t *struct_rec);

/* Register details within structure */
#define REG_STRUCT(struct_db, st_name, fields_arr)                    \
    do{                                                               \
        struct_db_rec_t *rec = calloc(1, sizeof(struct_db_rec_t));    \
        strncpy(rec->struct_name, #st_name, MAXIMUM_STRUCT_NAME_SIZE); \
        rec->ds_size = sizeof(st_name);                               \
        rec->n_fields = sizeof(fields_arr)/sizeof(field_info_t);      \
        rec->fields = fields_arr;                                     \
        if(add_structure_to_struct_db(struct_db, rec))                \
        {                                                             \
            assert(0);                                                \
        }                                                             \
    }while(0);

#endif /* __GCL_H */

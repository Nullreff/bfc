// Shared macros n stuff
#pragma once

#define CHECK_OOM(pointer) if (!pointer) { fprintf(stderr, "Out of memory!\n"); exit(EXIT_FAILURE); }
#define DATA_VAL(val) val->data[val->index]

typedef struct
{
    int index;
    int length;
    char* data;
} BF_Code;

typedef struct
{
    int debug;
    long data_length;
} BF_Options;


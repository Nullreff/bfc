// Shared macros n stuff
#pragma once

#define ERROR_IF(condition, message) if (condition) { fprintf(stderr, message); exit(EXIT_FAILURE); }
#define CHECK_OOM(pointer) ERROR_IF(!pointer, "Out of memory!\n")
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

typedef struct
{
    int index;
    int length;
    int* data;
    int* skips;
} BF_State;


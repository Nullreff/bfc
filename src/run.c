#include <stdio.h>
#include <stdlib.h>
#include "run.h"
#include "common.h"

#define DATA_VAL(val) (val).data[(val).index]

typedef struct
{
    int index;
    int length;
    int* data;
} BF_State;

typedef struct
{
    int index;
    int length;
    char* data;
} BF_Code;

int run_to_loop_end(BF_Code* code)
{
    int loops = 1;

    do
    {
        switch (DATA_VAL(*code))
        {
            case '[':
                loops++;
                break;
            case ']':
                loops--;
                break;
        }

        code->index++;
    }
    while (loops > 0);
}

void strip_comments(BF_Code* code)
{
    char* data = malloc(sizeof(char) * code->length);
    int i, j;

    for (i = j = 0; i < code->length; i++)
    {
        switch(code->data[i])
        {
            case '>':
            case '<':
            case '+':
            case '-':
            case '.':
            case ',':
            case '[':
            case ']':
                data[j] = code->data[i];
                j++;
                break;
        }
    }

    free(code->data);
    code->data = data;
    code->length = j;
}

void debug_info(BF_Code code, BF_State state)
{
    int i;

    printf(" ");
    for (i = 0; i < code.length; i++)
    {
        printf("%c", code.data[i]);
    }
    printf("\n%*s" "^\n", code.index + 1, " ");

    printf(" ");
    for (i = 0; i < 100; i++)
    {
        printf("%d", state.data[i]);
    }
    printf("\n%*s" "^\n", state.index + 1, " ");
}

void run_code(BF_Code* code, BF_State* state)
{
    int start = code->index;
    int i, j, end;

    for (; code->index < code->length; code->index++)
    {
        switch (DATA_VAL(*code))
        {
            case '+':
                DATA_VAL(*state)++;
                break;

            case '-':
                DATA_VAL(*state)--;
                break;

            case '>':
                state->index++;
                break;

            case '<':
                state->index--;
                break;

            case '.':
                putchar(DATA_VAL(*state));
                break;

            case ',':
                DATA_VAL(*state) = getchar();
                break;

            case '[':
                end = -1;

                if (DATA_VAL(*state) == 0)
                {
                    run_to_loop_end(code);
                    break;
                }

                do
                {
                    run_code(code, state);
                }
                while (DATA_VAL(*state) != 0);

                break;

            case ']':
                return;
        }
    }
}

void bf_run(char* program, long length)
{
    int* data = malloc(sizeof(int) * 30000);
    CHECK_OOM(data);

    BF_Code code = (BF_Code){0, 0, program};
    BF_State state = (BF_State){0, 0, data};

    run_code(&code, &state);

    free(data);
}

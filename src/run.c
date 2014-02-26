#include <stdio.h>
#include <stdlib.h>
#include "run.h"
#include "common.h"

#define DATA_VAL(val) val->data[val->index]
#define DATA_EACH(val) for (;val->index < val->length; val->index++)

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
        code->index++;

        switch (DATA_VAL(code))
        {
            case '[':
                loops++;
                break;
            case ']':
                loops--;
                break;
        }
    }
    while (loops > 0);
}

void strip_comments(BF_Code* code)
{
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
                if (i != j)
                {
                    code->data[j] = code->data[i];
                }
                j++;
                break;
        }
    }

    realloc(code->data, j);
    code->length = j;
}

void debug_info(BF_Code code, BF_State state)
{
    int i;

    printf("\n---\n ");
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
    printf("---\n");
}

void run_code(BF_Code* code, BF_State* state)
{
    int start;

    DATA_EACH(code)
    {
        switch (DATA_VAL(code))
        {
            case '+':
                DATA_VAL(state)++;
                break;

            case '-':
                DATA_VAL(state)--;
                break;

            case '>':
                state->index++;
                break;

            case '<':
                state->index--;
                break;

            case '.':
                printf("%c", DATA_VAL(state));
                break;

            case ',':
                DATA_VAL(state) = getchar();
                break;

            case '[':
                if (!DATA_VAL(state))
                {
                    run_to_loop_end(code);
                    break;
                }

                start = ++code->index;
                while(1)
                {
                    run_code(code, state);

                    if (!DATA_VAL(state))
                    {
                        break;
                    }

                    code->index = start;
                }
                break;

            case ']':
                return;
        }
    }
}

void bf_run(char* program, long program_length, long data_length)
{
    int* data = calloc(data_length, sizeof(int));
    CHECK_OOM(data);
    BF_Code* code = malloc(sizeof(BF_Code));
    CHECK_OOM(code);
    BF_State* state = malloc(sizeof(BF_State));
    CHECK_OOM(state);

    *code = (BF_Code){0, program_length, program};
    *state = (BF_State){0, data_length, data};

    strip_comments(code);
    run_code(code, state);

    free(state->data);
    free(code);
    free(state);
}

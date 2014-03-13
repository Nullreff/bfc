#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "run.h"
#include "common.h"

#define DATA_EACH(val) for (;val->index < val->length; val->index++)

void run_to_loop_end(BF_Code* code, BF_State* state)
{
    if (state->skips[code->index])
    {
        code->index = state->skips[code->index];
        return;
    }

    int start = code->index;
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

    state->skips[start] = code->index;
}

void run_loop(BF_Code* code, BF_State* state, int debug)
{
    int start = ++code->index;
    while(1)
    {
        bf_run_code(code, state, debug);

        if (!DATA_VAL(state))
        {
            break;
        }

        code->index = start;
    }
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
            case '#':
                if (i != j)
                {
                    code->data[j] = code->data[i];
                }
                j++;
                break;
        }
    }

    char* temp = realloc(code->data, j);
    CHECK_OOM(temp);
    code->data = temp;
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

void bf_run_code(BF_Code* code, BF_State* state, int debug)
{
    int start;

    DATA_EACH(code)
    {
        if (debug)
        {
            debug_info(*code, *state);
        }

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
                if (DATA_VAL(state))
                {
                    run_loop(code, state, debug);
                }
                else
                {
                    run_to_loop_end(code, state);
                }
                break;

            case ']':
                return;

            case '#':
                debug_info(*code, *state);
                break;
        }
    }
}

void bf_run(BF_Code code, BF_Options options)
{
    int* data = calloc(options.data_length, sizeof(int));
    CHECK_OOM(data);

    int* skips = calloc(options.data_length, sizeof(int));
    CHECK_OOM(skips);

    BF_Code* code_copy = malloc(sizeof(BF_Code));
    CHECK_OOM(code_copy);

    BF_State* state = malloc(sizeof(BF_State));
    CHECK_OOM(state);

    memcpy(code_copy, &code, sizeof(BF_Code));
    *state = (BF_State){0, options.data_length, data, skips};

    strip_comments(code_copy);
    bf_run_code(code_copy, state, options.debug);

    free(state->data);
    free(state->skips);
    free(state);
    free(code_copy->data);
    free(code_copy);
}

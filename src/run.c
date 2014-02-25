#include <stdio.h>
#include <stdlib.h>
#include "run.h"
#include "common.h"

int run_code(char* program, int start, int length, int* data)
{
    int i;
    int end;

    for (i = start; i < length; i++)
    {
        char value = program[i];
        switch (value)
        {
            case '+':
                ++*data;
                break;

            case '-':
                --*data;
                break;

            case '>':
                ++data;
                break;

            case '<':
                --data;
                break;

            case '.':
                putchar(*data);
                break;

            case ',':
                *data = getchar();
                break;

            case '[':
                while (*data)
                {
                    end = run_code(program, i + 1, length, data);
                }
                i = end;
                break;

            case ']':
                return i;
        }
    }

    return i;
}

void bf_run(char* program, long length)
{
    int* data = malloc(sizeof(int) * 30000);
    CHECK_OOM(data);

    run_code(program, 0, length, data);

    free(data);
}

#include <stdio.h>
#include <stdlib.h>

#define CHECK_OOM(pointer) if (!pointer) { fprintf(stderr, "Out of memory!\n"); exit(EXIT_FAILURE); }
int* data;
int* start;
char* code;

int run_code(int start)
{
    int i = start;
    int end;

    while (code[i] != '\0')
    {
        switch (code[i])
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
                    end = run_code(i + 1);
                }
                i = end;
                break;

            case ']':
                return i;
                break;
        }

        i++;
    }

    return i;
}

int main(int argc, char* argv[])
{
    int size = 30000;

    start = data = malloc(sizeof(int) * size);
    CHECK_OOM(data);
    code = argv[1];
    run_code(0);
    free(start);
}

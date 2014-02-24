#include <stdio.h>
#include <stdlib.h>

#define FAIL_IF(test) if (test) { exit(EXIT_FAILURE); }

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
        }

        i++;
    }

    return i;
}

int main(int argc, char* argv[])
{
    FAIL_IF(argc != 2);

    code = argv[1];
    start = data = malloc(sizeof(int) * 30000);
    FAIL_IF(!data);

    run_code(0);

    free(start);
    return EXIT_SUCCESS;
}

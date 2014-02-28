#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "build.h"
#include "run.h"
#include "common.h"

void print_help(char* name)
{
    printf("Usage: %s <commmand> [args]\n", name);
    printf("Commands:\n");
    printf("    run   <file> [--debug] - Executes the contents of the file\n");
    printf("    build <file>           - Compiles the file into an executable\n");
    printf("    help                   - Prints this message\n");
}

BF_Code get_code_from_file(char* name)
{
    FILE* file;
    BF_Code code = {0, 0, NULL};

    file = fopen(name, "r");
    ERROR_IF(!file, "Error opening file\n")

    fseek(file, 0, SEEK_END);
    code.length = ftell(file);

    fseek(file, 0, SEEK_SET);
    code.data = malloc(code.length);
    CHECK_OOM(code.data);

    fread(code.data, 1, code.length, file);
    fclose(file);

    return code;
}

int main(int argc, char* argv[])
{
    void (*command)(BF_Code, BF_Options);
    BF_Options options = (BF_Options){0, 30000};
    BF_Code code;

    if (argc == 2 && strcmp(argv[1], "help") == 0)
    {
        print_help(argv[0]);
        exit(EXIT_SUCCESS);
    }
    else if (argc >= 3 && strcmp(argv[1], "run") == 0)
    {
        command = bf_run;
        if (argc == 4 && strcmp(argv[3], "--debug") == 0)
        {
            options.debug = 1;
        }
    }
    else if (argc == 3 && strcmp(argv[1], "build") == 0)
    {
        command = bf_build;
    }
    else
    {
        fprintf(stderr, "Invalid arguments, try '%s help'\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    code = get_code_from_file(argv[2]);
    command(code, options);
    free(code.data);
}

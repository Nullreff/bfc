#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "build.h"
#include "run.h"

#define CHECK_OOM(pointer) if (!pointer) { fprintf(stderr, "Out of memory!\n"); exit(EXIT_FAILURE); }

void print_help(char* name)
{
    printf("Usage: %s <commmand> [args]\n", name);
    printf("Commands:\n");
    printf("    run   <file> - Executes the contents of the file\n");
    printf("    build <file> - Compiles the file into an executable\n");
    printf("    help         - Prints this message\n");
}

long get_file_contents(char* name, char** buffer)
{
    long length;
    FILE* file;

    file = fopen (name, "r");
    fseek(file, 0, SEEK_END);
    length = ftell(file);

    fseek(file, 0, SEEK_SET);
    *buffer = malloc(length);
    CHECK_OOM(buffer);

    fread(*buffer, 1, length, file);
    fclose(file);

    return length;
}

int main(int argc, char* argv[])
{
    void (*command)(char*, long);
    char* program = 0;
    long length = 0;

    if (argc == 2 && strcmp(argv[1], "help") == 0)
    {
        print_help(argv[0]);
        exit(EXIT_SUCCESS);
    }
    else if (argc == 3 && strcmp(argv[1], "run") == 0)
    {
        command = bf_run;
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

    length = get_file_contents(argv[2], &program);
    command(program, length);
    free(program);
}

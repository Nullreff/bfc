#include <stdio.h>
#include <stdlib.h>

#define CHECK_OOM(pointer) if (!pointer) { fprintf(stderr, "Out of memory!\n"); exit(EXIT_FAILURE); }

typedef enum 
{
    // Simple Operators
    BF_NOOP,
    BF_SHIFT,
    BF_MODIFY,
    BF_PRINT,
    BF_READ,
    BF_LOOP,
    BF_LOOP_END,
} BF_Operation;

static const char *OP_NAMES[] = { "noop", "shift", "modify", "print", "read", "loop", "loop_end"};

typedef struct
{
    BF_Operation op;
    int value;
} Instruction;

void print_instruction_indent(Instruction inst, int indentation)
{
    printf("%*s" "%s %d\n", indentation, " ", OP_NAMES[inst.op], inst.value);
}

void print_instruction(Instruction inst)
{
    if (inst.op != BF_NOOP)
    {
        print_instruction_indent(inst, 2);
    }
}

Instruction new_instruction(int c)
{
    Instruction inst = {BF_NOOP, 0};

    switch (c)
    {
        case '>':
            inst.op = BF_SHIFT;
            inst.value = 1;
            break;
        case '<':
            inst.op = BF_SHIFT;
            inst.value = -1;
            break;
        case '+':
            inst.op = BF_MODIFY;
            inst.value = 1;
            break;
        case '-':
            inst.op = BF_MODIFY;
            inst.value = -1;
            break;
        case '.':
            inst.op = BF_PRINT;
            inst.value = 1;
            break;
        case ',':
            inst.op = BF_READ;
            inst.value = 1;
            break;
        case '[':
            inst.op = BF_LOOP;
            break;
        case ']':
            inst.op = BF_LOOP_END;
            break;
        default:
            inst.op = BF_NOOP;
            break;
    }

    return inst;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s file.bf\n", argv[0]);
        exit(1);
    }

    int c;
    int index = -1;
    int size = 10;
    FILE* file = fopen (argv[1], "r");
    Instruction* instructions = malloc(sizeof(Instruction) * size);
    CHECK_OOM(instructions);

    // Read in instructions
    while ((c = fgetc(file)) != EOF)
    {
        Instruction newInst = new_instruction(c);

        if (newInst.op == BF_NOOP)
        {
            continue;
        }
        else if (index == -1)
        {
            instructions[0] = newInst;
            index++;
        }
        else if (newInst.op != instructions[index].op)
        {
            index++;

            // Reallocate array if we run out of room
            if (index >= size)
            {
                Instruction* temp = realloc(instructions, sizeof(Instruction) * size * 2);
                CHECK_OOM(temp);
                size *= 2;
                instructions = temp;
            }

            instructions[index] = newInst;
        }
        else
        {
            instructions[index].value += newInst.value;
        }
    }

    int count = index + 1;

    printf("Instructions:\n");
    for (index = 0; index < count; index++)
    {
        print_instruction(instructions[index]);
    }

    free(instructions);
}

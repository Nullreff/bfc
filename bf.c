#include <stdio.h>
#include <stdlib.h>

#define FOREACH(type,item,list) for (type* item = list;item;item = item->next)

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

typedef struct instruction_
{
    BF_Operation op;
    int value;
    struct instruction_* next;
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

Instruction get_instruction(char c)
{
    switch (c)
    {
        case '>':
            return (Instruction){BF_SHIFT, 1};
        case '<':
            return (Instruction){BF_SHIFT, -1};
        case '+':
            return (Instruction){BF_MODIFY, 1};
        case '-':
            return (Instruction){BF_MODIFY, -1};
        case '.':
            return (Instruction){BF_PRINT, 1};
        case ',':
            return (Instruction){BF_READ, 1};
        case '[':
            return (Instruction){BF_LOOP};
        case ']':
            return (Instruction){BF_LOOP_END};
        default:
            return (Instruction){BF_NOOP};
    }
}

int main()
{
    int c;
    Instruction* currentInst = NULL;
    Instruction* headInst = NULL;

    // Read in instructions
    while ((c = getchar()) != EOF)
    {
        Instruction* inst = malloc(sizeof(Instruction));
        *inst = get_instruction(c);

        if (inst->op == BF_NOOP)
        {
            continue;
        }

        if (currentInst == NULL)
        {
            currentInst = inst;
            headInst = currentInst;
        }
        else if (inst->op != currentInst->op)
        {
            currentInst->next = inst;
            currentInst = inst;
        }
        else
        {
            currentInst->value += inst->value;
        }
    }

    printf("Instructions:\n");
    FOREACH(Instruction, inst, headInst)
    {
        print_instruction(*inst);
    }
}

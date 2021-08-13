#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "insset.h"

#define MAX_INSTRUCTIONS 100

struct ins {
    char name[20];
    uint16_t op;
    uint16_t op_mask;
};

static struct {
    size_t n;
    struct ins ins[MAX_INSTRUCTIONS];
} set;

static size_t count_bits(uint16_t x)
{
    size_t n = 0;
    while (x != 0) {
        if (x & 1)
            ++n;
        x >>= 1;
    }
    return n;
}

void init_instruction_set(void)
{
    memset(&set, 0, sizeof(set));
}

bool add_instruction(const char* name, uint16_t op, uint16_t op_mask)
{
    if (set.n == MAX_INSTRUCTIONS)
        return false;
    struct ins* ins = &set.ins[set.n++];
    strncpy(ins->name, name, sizeof(ins->name));
    ins->op = op;
    ins->op_mask = op_mask;
    return true;
}

int decode_instruction(uint16_t op)
{
    int d = -1;
    for (int i = 0; i < set.n; ++i) {
        const struct ins* ins = &set.ins[i];
        if ((op & ins->op_mask) == ins->op) {
            if (d == -1 || count_bits(ins->op_mask) > count_bits(set.ins[d].op_mask))
                d = i;
        }
    }
    return d;
}

size_t get_instruction_count(void)
{
    return set.n;
}

const char* get_instruction_name(size_t index)
{
    return set.ins[index].name;
}

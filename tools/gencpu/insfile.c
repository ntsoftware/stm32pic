#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "insfile.h"
#include "insset.h"

struct ins {
    uint16_t op_mask;
    uint16_t op;
    uint16_t d;
    uint16_t f;
    uint16_t b;
    uint16_t k;
    uint16_t x;
};

static bool parse_instruction(const char* fmt, struct ins* ins)
{
    size_t n = strlen(fmt);
    uint16_t bit = 1;
    ins->op_mask = 0;
    ins->op = 0;
    ins->d = 0;
    ins->f = 0;
    ins->b = 0;
    ins->k = 0;
    ins->x = 0;
    for (int i = 0; i < n; ++i) {
        char c = fmt[n - i - 1];
        if (!isspace(c)) {
            if (c == '0' || c == '1') {
                ins->op_mask |= bit;
                ins->op |= (c == '1') ? bit : 0;
            }
            else {
                if (c == 'd')
                    ins->d |= bit;
                else if (c == 'f')
                    ins->f |= bit;
                else if (c == 'b')
                    ins->b |= bit;
                else if (c == 'k')
                    ins->k |= bit;
                else if (c == 'x')
                    ins->x |= bit;
                else
                    return false;
            }
            bit <<= 1;
        }
    }
    return true;
}

static bool process_instruction(const char* name, const struct ins* ins)
{
    if (ins->d) {
        char variant[20];
        snprintf(variant, sizeof(variant), "%s_0", name);
        if (!add_instruction(variant, ins->op, ins->op_mask | ins->d))
            return false;
        snprintf(variant, sizeof(variant), "%s_1", name);
        if (!add_instruction(variant, ins->op | ins->d, ins->op_mask | ins->d))
            return false;
    }
    else {
        if (!add_instruction(name, ins->op, ins->op_mask))
            return false;
    }
    return true;
}

bool load_instruction_set(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if (f == NULL)
        return false;
    bool ret = true;
    char line[100];
    while (fgets(line, sizeof(line), f)) {
        struct ins ins;
        const char* name = strtok(line, "\t\r\n");
        if (name == NULL) {
            ret = false;
            break;
        }
        const char* fmt = strtok(NULL, "\t\r\n");
        if (fmt == NULL || !parse_instruction(fmt, &ins)) {
            ret = false;
            break;
        }
        if (!process_instruction(name, &ins)) {
            ret = false;
            break;
        }
    }
    fclose(f);
    return ret;
}

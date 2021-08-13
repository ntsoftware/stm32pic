#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "args.h"
#include "insfile.h"
#include "insset.h"

static void print_dec(void)
{
    for (uint16_t op = 0; op < 0x1000; ++op) {
        int i = decode_instruction(op);
        if (i < 0)
            exit_with_error(1, "No instruction for opcode %04x\n", op);
        if ((op % 16) == 0)
            printf("/* %04x */  ", op);
        printf("%2d,", i);
        if ((op % 16) == 15)
            putchar('\n');
        else
            putchar(' ');
    }
}

static void print_lut(void)
{
    size_t n = get_instruction_count();
    for (int i = 0; i < n; ++i)
        printf("/* %2d */ %s,\n", i, get_instruction_name(i));
}

int main(int argc, char* argv[])
{
    parse_args(argc, argv);
    init_instruction_set();
    if (!load_instruction_set(args.file))
        exit_with_error(1, "Error loading file %s\n", args.file);
    if (args.cmd == CMD_DEC)
        print_dec();
    else if (args.cmd == CMD_LUT)
        print_lut();
    return 0;
}

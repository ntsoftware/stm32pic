#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "args.h"
#include "insfile.h"
#include "insset.h"

static void cmd_dec(void)
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

static void cmd_lut(void)
{
    size_t n = get_instruction_count();
    for (int i = 0; i < n; ++i)
        printf("/* %2d */ %s,\n", i, get_instruction_name(i));
}

static void cmd_swap(void)
{
    for (int i = 0; i < 256; ++i) {
        if ((i % 16) == 0)
            printf("/* %02x */  ", i);
        uint8_t hi = i & 0xf0;
        uint8_t lo = i & 0x0f;
        uint8_t x = (lo << 4) | (hi >> 4);
        printf("0x%02x,", x);
        if ((i % 16) == 15)
            putchar('\n');
        else
            putchar(' ');
    }
}

static void cmd_bit(void)
{
    for (int i = 0; i < 256; ++i) {
        if ((i % 16) == 0)
            printf("/* %02x */  ", i);
        uint8_t b = (i & 0xe0) >> 5;
        uint8_t x = 1 << b;
        printf("0x%02x,", x);
        if ((i % 16) == 15)
            putchar('\n');
        else
            putchar(' ');
    }
}

static void run_command(enum cmd cmd)
{
    if (cmd == CMD_DEC)
        cmd_dec();
    else if (cmd == CMD_LUT)
        cmd_lut();
    else if (cmd == CMD_SWAP)
        cmd_swap();
    else if (cmd == CMD_BIT)
        cmd_bit();
}

int main(int argc, char* argv[])
{
    parse_args(argc, argv);
    init_instruction_set();
    if (!load_instruction_set(args.file))
        exit_with_error(1, "Error loading file %s\n", args.file);
    run_command(args.cmd);
    return 0;
}

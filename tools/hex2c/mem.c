#include <stdint.h>
#include <stdio.h>

#include "mem.h"

uint16_t mem[MEM_WORDS];

void init_mem(void)
{
    for (int i = 0; i < MEM_WORDS; ++i)
        mem[i] = 0x3fff;
}

void dump_mem(void)
{
    for (int i = 0; i < MEM_WORDS; ++i) {
        if ((i % 8) == 0)
            printf("/* %04x */  ", i);
        printf("0x%04x,", mem[i]);
        if ((i % 8) == 7)
            putchar('\n');
        else
            putchar(' ');
    }
}

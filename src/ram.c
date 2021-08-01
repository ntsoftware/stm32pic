#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define RAM_SIZE 32

static uint8_t ram[RAM_SIZE];

void init_ram(void)
{
    memset(ram, 0, sizeof(ram));
}

void wr(uint8_t reg, uint8_t value)
{
    ram[reg] = value;
}

uint8_t rd(uint8_t reg)
{
    return ram[reg];
}

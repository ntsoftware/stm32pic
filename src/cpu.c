#include <stdio.h>
#include <stdint.h>

#include "flash.h"
#include "ram.h"

static struct {
    uint16_t pc;
    uint8_t w;
    int z;
    uint8_t tris;
    uint8_t option;
} cpu;

void init_cpu(void)
{
    cpu.pc = 0;
    cpu.w = 0;
    cpu.z = 0;
    cpu.tris = 0x3f;
    cpu.option = 0xff;
}

void step(void)
{
    uint16_t op = flash[cpu.pc];
    ++cpu.pc;
    /* CLRF f */
    if ((op & 0xfe0) == 0x060) {
        wr(op & 0x1f, 0);
        cpu.z = 1;
    }
    /* MOVWF f */
    else if ((op & 0xfe0) == 0x020) {
        wr(op & 0x1f, cpu.w);
    }
    /* DECFSZ f,0 */
    else if ((op & 0xfe0) == 0x2c0) {
        uint8_t f = op & 0x1f;
        uint8_t x = rd(f) - 1;
        cpu.w = x;
        if (x == 0)
            ++cpu.pc;
    }
    /* DECFSZ f,1 */
    else if ((op & 0xfe0) == 0x2e0) {
        uint8_t f = op & 0x1f;
        uint8_t x = rd(f) - 1;
        wr(f, x);
        if (x == 0)
            ++cpu.pc;
    }
    /* MOVLW k */
    else if ((op & 0xf00) == 0xc00) {
        cpu.w = op & 0xff;
    }
    /* GOTO k */
    else if ((op & 0xe00) == 0xa00) {
        cpu.pc = op & 0x1ff;
    }
    /* TRIS f (f=6) */
    else if (op == 0x006) {
        cpu.tris = cpu.w;
    }
    else {
        /* unknown opcode */
    }
}

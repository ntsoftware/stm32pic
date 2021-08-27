#include <stdint.h>
#include <stdio.h>

#include "cpu.h"
#include "flash.h"
#include "regs.h"
#include "sfr.h"

/*
This implementation uses lazy processor flags computation, i.e. flags are
computed only when needed. The following cpu struct members are used to track
intermediate results of operations.

  a  first operand of the last add/sub operation
  b  second operand of the last add/sub operation
  x  result of the last operation that involves the carry flag
  y  result of the last operation that involves the zero flag

*/

static struct {
    uint8_t status;
    uint16_t pc;
    uint8_t w;
    int a;
    int b;
    int x;
    uint8_t y;
    uint16_t stack[2];
    unsigned cycles;
} cpu;

static void unk(uint16_t op)
{
    /* unknown instruction */
}

static void addwf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.a = cpu.w;
    cpu.b = rd(f);
    cpu.x = cpu.a + cpu.b;
    cpu.y = cpu.x & 0xff;
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void addwf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.a = cpu.w;
    cpu.b = rd(f);
    cpu.x = cpu.a + cpu.b;
    cpu.y = cpu.x & 0xff;
    wr(f, cpu.y);
    cpu.cycles += 1;
}

static void andwf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) & cpu.w;
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void andwf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) & cpu.w;
    wr(f, cpu.y);
    cpu.cycles += 1;
}

static void clrf(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = 0;
    wr(f, 0);
    cpu.cycles += 1;
}

static void clrw(uint16_t op)
{
    cpu.y = 0;
    cpu.w = 0;
    cpu.cycles += 1;
}

static void comf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = ~rd(f);
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void comf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = ~rd(f);
    wr(f, cpu.y);
    cpu.cycles += 1;
}

static void decf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) - 1;
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void decf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) - 1;
    wr(f, cpu.y);
    cpu.cycles += 1;
}

static void decfsz_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f) - 1;
    cpu.w = y;
    if (y == 0)
        ++cpu.pc;
    cpu.cycles += 1;
}

static void decfsz_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f) - 1;
    wr(f, y);
    if (y == 0)
        ++cpu.pc;
    cpu.cycles += 1;
}

static void incf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) + 1;
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void incf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) + 1;
    wr(f, cpu.y);
    cpu.cycles += 1;
}

static void incfsz_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f) + 1;
    cpu.w = y;
    if (y == 0)
        ++cpu.pc;
    cpu.cycles += 1;
}

static void incfsz_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f) + 1;
    wr(f, y);
    if (y == 0)
        ++cpu.pc;
    cpu.cycles += 1;
}

static void iorwf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) | cpu.w;
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void iorwf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) | cpu.w;
    wr(f, cpu.y);
    cpu.cycles += 1;
}

static void movf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f);
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void movf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f);
    wr(f, cpu.y);
    cpu.cycles += 1;
}

static void movwf(uint16_t op)
{
    uint8_t f = op & 0x1f;
    wr(f, cpu.w);
    cpu.cycles += 1;
}

static void nop(uint16_t op)
{
    cpu.cycles += 1;
}

static void rlf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t x = rd(f);
    uint8_t y = x << 1;
    if ((cpu.x & 0x0100) != 0)
        y |= 0x01;
    if ((x & 0x80) != 0)
        cpu.x |= 0x0100;
    else
        cpu.x &= ~0x0100;
    cpu.w = y;
    cpu.cycles += 1;
}

static void rlf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t x = rd(f);
    uint8_t y = x << 1;
    if ((cpu.x & 0x0100) != 0)
        y |= 0x01;
    if ((x & 0x80) != 0)
        cpu.x |= 0x0100;
    else
        cpu.x &= ~0x0100;
    wr(f, y);
    cpu.cycles += 1;
}

static void rrf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t x = rd(f);
    uint8_t y = x >> 1;
    if ((cpu.x & 0x0100) != 0)
        y |= 0x80;
    if ((x & 0x01) != 0)
        cpu.x |= 0x0100;
    else
        cpu.x &= ~0x0100;
    cpu.w = y;
    cpu.cycles += 1;
}

static void rrf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t x = rd(f);
    uint8_t y = x >> 1;
    if ((cpu.x & 0x0100) != 0)
        y |= 0x80;
    if ((x & 0x01) != 0)
        cpu.x |= 0x0100;
    else
        cpu.x &= ~0x0100;
    wr(f, y);
    cpu.cycles += 1;
}

static void subwf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.a = cpu.w;
    cpu.b = -rd(f);
    cpu.x = cpu.a + cpu.b;
    cpu.y = cpu.x & 0xff;
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void subwf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.a = cpu.w;
    cpu.b = -rd(f);
    cpu.x = cpu.a + cpu.b;
    cpu.y = cpu.x & 0xff;
    wr(f, cpu.y);
    cpu.cycles += 1;
}

static const uint8_t swap_lut[] = {
#include "swap_lut.h"
};

static void swapf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = swap_lut[rd(f)];
    cpu.w = y;
    cpu.cycles += 1;
}

static void swapf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = swap_lut[rd(f)];
    wr(f, y);
    cpu.cycles += 1;
}

static void xorwf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) ^ cpu.w;
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void xorwf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) ^ cpu.w;
    wr(f, cpu.y);
    cpu.cycles += 1;
}

/* bit-oriented file register operations */

static const uint8_t bit_lut[] = {
#include "bit_lut.h"
};

static void bcf(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f) & ~bit_lut[op & 0xff];
    wr(f, y);
    cpu.cycles += 1;
}

static void bsf(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f) | bit_lut[op & 0xff];
    wr(f, y);
    cpu.cycles += 1;
}

static void btfsc(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f);
    if ((y & bit_lut[op & 0xff]) == 0)
        ++cpu.pc;
    cpu.cycles += 1;
}

static void btfss(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f);
    if ((y & bit_lut[op & 0xff]) != 0)
        ++cpu.pc;
    cpu.cycles += 1;
}

/* literal and control operations */

static void andlw(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.y = cpu.w & k;
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void call(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.stack[1] = cpu.stack[0];
    cpu.stack[0] = cpu.pc;
    cpu.pc = k;
    cpu.cycles += 2;
}

static void clrwdt(uint16_t op)
{
    /* not implemented */
    cpu.cycles += 1;
}

static void _goto(uint16_t op)
{
    uint16_t k = op & 0x1ff;
    cpu.pc = k;
    cpu.cycles += 2;
}

static void iorlw(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.y = cpu.w | k;
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static void movlw(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.w = k;
    cpu.cycles += 1;
}

static void option(uint16_t op)
{
    sfr_option.wr(cpu.w);
    cpu.cycles += 1;
}

static void retlw(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.pc = cpu.stack[0];
    cpu.stack[0] = cpu.stack[1];
    cpu.w = k;
    cpu.cycles += 2;
}

static void sleep(uint16_t op)
{
    /* not implemented */
    cpu.cycles += 1;
}

static void tris(uint16_t op)
{
    uint8_t f = op & 7;
    if (f == 6)
        sfr_trisgpio.wr(cpu.w);
    else
        unk(op);
    cpu.cycles += 1;
}

static void xorlw(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.y = cpu.w ^ k;
    cpu.w = cpu.y;
    cpu.cycles += 1;
}

static const uint8_t ins_dec[] = {
#include "baseline_dec.h"
};

static const void (*ins_lut[])(uint16_t) = {
#include "baseline_lut.h"
};

void init_cpu(void)
{
    cpu.status = STATUS_TO | STATUS_PD;
    cpu.pc = 0;
    cpu.w = 0;
    cpu.a = 0;
    cpu.b = 0;
    cpu.x = 0;
    cpu.y = 0;
    cpu.cycles = 0;
}

void step(void)
{
    uint16_t op = flash[cpu.pc++];
    uint8_t i = ins_dec[op];
    ins_lut[i](op);
}

uint16_t get_pc(void)
{
    return cpu.pc;
}

uint8_t get_status(void)
{
    uint8_t value = cpu.status;
    if (cpu.y == 0)
        value |= STATUS_Z;
    if ((cpu.x & 0x0100) != 0)
        value |= STATUS_C;
    if (((cpu.x ^ cpu.a ^ cpu.b) & 0x0010) != 0)
        value |= STATUS_DC;
    return value;
}

unsigned get_cycles(void)
{
    return cpu.cycles;
}

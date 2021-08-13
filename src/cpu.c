#include <stdint.h>
#include <stdio.h>

#include "cpu.h"
#include "flash.h"
#include "regs.h"

static struct {
    uint16_t pc;
    uint8_t w;
    int a;
    int b;
    int x;
    uint8_t y;
    uint16_t stack[2];
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
}

static void addwf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.a = cpu.w;
    cpu.b = rd(f);
    cpu.x = cpu.a + cpu.b;
    cpu.y = cpu.x & 0xff;
    wr(f, cpu.y);
}

static void andwf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) & cpu.w;
    cpu.w = cpu.y;
}

static void andwf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) & cpu.w;
    wr(f, cpu.y);
}

static void clrf(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = 0;
    wr(f, 0);
}

static void clrw(uint16_t op)
{
    cpu.y = 0;
    cpu.w = 0;
}

static void comf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = ~rd(f);
    cpu.w = cpu.y;
}

static void comf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = ~rd(f);
    wr(f, cpu.y);
}

static void decf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) - 1;
    cpu.w = cpu.y;
}

static void decf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) - 1;
    wr(f, cpu.y);
}

static void decfsz_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f) - 1;
    cpu.w = y;
    if (y == 0)
        ++cpu.pc;
}

static void decfsz_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f) - 1;
    wr(f, y);
    if (y == 0)
        ++cpu.pc;
}

static void incf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) + 1;
    cpu.w = cpu.y;
}

static void incf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) + 1;
    wr(f, cpu.y);
}

static void incfsz_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f) + 1;
    cpu.w = y;
    if (y == 0)
        ++cpu.pc;
}

static void incfsz_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f) + 1;
    wr(f, y);
    if (y == 0)
        ++cpu.pc;
}

static void iorwf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) | cpu.w;
    cpu.w = cpu.y;
}

static void iorwf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) | cpu.w;
    wr(f, cpu.y);
}

static void movf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f);
    cpu.w = cpu.y;
}

static void movf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f);
    wr(f, cpu.y);
}

static void movwf(uint16_t op)
{
    uint8_t f = op & 0x1f;
    wr(f, cpu.w);
}

static void nop(uint16_t op)
{
    /* nop */
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
}

static void subwf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.a = cpu.w;
    cpu.b = -rd(f);
    cpu.x = cpu.a + cpu.b;
    cpu.y = cpu.x & 0xff;
    cpu.w = cpu.y;
}

static void subwf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.a = cpu.w;
    cpu.b = -rd(f);
    cpu.x = cpu.a + cpu.b;
    cpu.y = cpu.x & 0xff;
    wr(f, cpu.y);
}

static const uint8_t swap_lut[] = {
#include "swap_lut.h"
};

static void swapf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = swap_lut[rd(f)];
    cpu.w = y;
}

static void swapf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = swap_lut[rd(f)];
    wr(f, y);
}

static void xorwf_0(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) ^ cpu.w;
    cpu.w = cpu.y;
}

static void xorwf_1(uint16_t op)
{
    uint8_t f = op & 0x1f;
    cpu.y = rd(f) ^ cpu.w;
    wr(f, cpu.y);
}

/* bit-oriented file register operations */

static const uint8_t bit_lut[] = {
#include "bit_lut.h"
};

static void bcf(uint16_t op)
{
    uint8_t f = op & 0x1f;
    clr_bits(f, bit_lut[op & 0xff]);
}

static void bsf(uint16_t op)
{
    uint8_t f = op & 0x1f;
    set_bits(f, bit_lut[op & 0xff]);
}

static void btfsc(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f);
    if ((y & bit_lut[op & 0xff]) == 0)
        ++cpu.pc;
}

static void btfss(uint16_t op)
{
    uint8_t f = op & 0x1f;
    uint8_t y = rd(f);
    if ((y & bit_lut[op & 0xff]) != 0)
        ++cpu.pc;
}

/* literal and control operations */

static void andlw(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.y = cpu.w & k;
    cpu.w = cpu.y;
}

static void call(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.stack[1] = cpu.stack[0];
    cpu.stack[0] = cpu.pc;
    cpu.pc = k;
}

static void clrwdt(uint16_t op)
{

}

static void _goto(uint16_t op)
{
    uint16_t k = op & 0x1ff;
    cpu.pc = k;
}

static void iorlw(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.y = cpu.w | k;
    cpu.w = cpu.y;
}

static void movlw(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.w = k;
}

static void option(uint16_t op)
{
    set_option(cpu.w);
}

static void retlw(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.pc = cpu.stack[0];
    cpu.stack[0] = cpu.stack[1];
    cpu.w = k;
}

static void sleep(uint16_t op)
{

}

static void tris(uint16_t op)
{
    uint8_t f = op & 7;
    if (f == 6)
        set_tris(cpu.w);
    else
        unk(op);
}

static void xorlw(uint16_t op)
{
    uint8_t k = op & 0xff;
    cpu.y = cpu.w ^ k;
    cpu.w = cpu.y;
}

typedef void (*ins_func_t)(uint16_t);

static const uint8_t dec[] = {
#include "baseline_dec.h"
};

static const ins_func_t lut[] = {
#include "baseline_lut.h"
};

void init_cpu(void)
{
    cpu.pc = 0;
    cpu.w = 0;
    cpu.a = 0;
    cpu.b = 0;
    cpu.x = 0;
    cpu.y = 0;
}

void step(void)
{
    uint16_t op = flash[cpu.pc++];
    uint8_t i = dec[op];
    lut[i](op);
}

uint16_t get_pc(void)
{
    return cpu.pc;
}

uint8_t get_status(void)
{
    uint8_t value = STATUS_TO | STATUS_PD;
    if (cpu.y == 0)
        value |= STATUS_Z;
    if ((cpu.x & 0x0100) != 0)
        value |= STATUS_C;
    if (((cpu.x ^ cpu.a ^ cpu.b) & 0x0010) != 0)
        value |= STATUS_DC;
    return value;
}

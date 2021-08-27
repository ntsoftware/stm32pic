#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "regs.h"

#define NUM_REGS 32

static struct {
    uint8_t tmr0;
    uint8_t fsr;
    uint8_t osccal;
    uint8_t gpio;
    uint8_t tris;
    uint8_t option;
} sfr;

static void init_sfr(void)
{
    sfr.tmr0 = 0;
    sfr.fsr = 0xe0;
    sfr.osccal = 0xfe;
    sfr.gpio = 0;
    sfr.tris = 0x3f;
    sfr.option = 0xff;
}

static uint8_t rd_indf(void)
{
    if (sfr.fsr == 0)
        return 0;
    return rd(sfr.fsr & 0x1f);
}

static void wr_indf(uint8_t value)
{
    if (sfr.fsr == 0)
        return;
    wr(sfr.fsr & 0x1f, value);
}

static uint8_t rd_tmr0(void)
{
    return sfr.tmr0;
}

static void wr_tmr0(uint8_t value)
{
    sfr.tmr0 = value;
}

static uint8_t rd_pcl(void)
{
    return get_pc() & 0xff;
}

static void wr_pcl(uint8_t value)
{
    /* not implemented */
}

static uint8_t rd_status(void)
{
    return get_status();
}

static void wr_status(uint8_t value)
{
    /* not implemented */
}

static uint8_t rd_fsr(void)
{
    return sfr.fsr;
}

static void wr_fsr(uint8_t value)
{
    sfr.fsr = value;
}

static uint8_t rd_osccal(void)
{
    return sfr.osccal;
}

static void wr_osccal(uint8_t value)
{
    sfr.osccal = value;
}

static uint8_t rd_gpio(void)
{
    return sfr.gpio;
}

static void wr_gpio(uint8_t value)
{
    sfr.gpio = value;
}

#define NUM_SFR 7

typedef uint8_t (*rd_sfr_func_t)(void);
typedef void (*wr_sfr_func_t)(uint8_t);

static const rd_sfr_func_t rd_sfr_lut[NUM_SFR] = {
    /* 0 */  rd_indf,
    /* 1 */  rd_tmr0,
    /* 2 */  rd_pcl,
    /* 3 */  rd_status,
    /* 4 */  rd_fsr,
    /* 5 */  rd_osccal,
    /* 6 */  rd_gpio,
};

static const wr_sfr_func_t wr_sfr_lut[NUM_SFR] = {
    /* 0 */  wr_indf,
    /* 1 */  wr_tmr0,
    /* 2 */  wr_pcl,
    /* 3 */  wr_status,
    /* 4 */  wr_fsr,
    /* 5 */  wr_osccal,
    /* 6 */  wr_gpio,
};

static uint8_t regs[NUM_REGS - NUM_SFR];

void init_regs(void)
{
    memset(regs, 0, sizeof(regs));
    init_sfr();
}

uint8_t rd(uint8_t reg)
{
    if (reg < NUM_SFR)
        return rd_sfr_lut[reg]();
    else
        return regs[reg - NUM_SFR];
}

void wr(uint8_t reg, uint8_t value)
{
    if (reg < NUM_SFR)
        wr_sfr_lut[reg](value);
    else
        regs[reg - NUM_SFR] = value;
}

void set_tris(uint8_t value)
{
    sfr.tris = value & 0x3f;
}

void set_option(uint8_t value)
{
    sfr.option = value;
}

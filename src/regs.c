#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "regs.h"
#include "sfr.h"

#define NUM_SFR 7

static const struct sfr* sfr_table[NUM_SFR] = {
    &sfr_indf,
    &sfr_tmr0,
    &sfr_pcl,
    &sfr_status,
    &sfr_fsr,
    &sfr_osccal,
    &sfr_gpio,
};

#define NUM_REGS 32

static uint8_t regs[NUM_REGS - NUM_SFR];

void init_regs(void)
{
    memset(regs, 0, sizeof(regs));
    for (int reg = 0; reg < NUM_SFR; ++reg)
        if (sfr_table[reg]->init != NULL)
            sfr_table[reg]->init();
}

uint8_t rd(uint8_t reg)
{
    if (reg < NUM_SFR)
        return sfr_table[reg]->rd();
    else
        return regs[reg - NUM_SFR];
}

void wr(uint8_t reg, uint8_t value)
{
    if (reg < NUM_SFR)
        sfr_table[reg]->wr(value);
    else
        regs[reg - NUM_SFR] = value;
}

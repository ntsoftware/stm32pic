#include <stdint.h>
#include <stdio.h>

#include "cpu.h"
#include "hw/gpio.h"
#include "hw/option.h"
#include "hw/tmr0.h"
#include "hw/wdt.h"
#include "regs.h"
#include "sfr.h"

/*
Implementation of the host machine independent special function registers.
Other registers are implemented in separate compilation units, with a
different version for each supported target.
*/

/* FSR register */

static uint8_t fsr;

static void init_fsr(void)
{
    fsr = 0xff;
}

static uint8_t rd_fsr(void)
{
    return fsr;
}

static void wr_fsr(uint8_t value)
{
    fsr = value;
}

const struct sfr sfr_fsr = {
    .init = init_fsr,
    .rd = rd_fsr,
    .wr = wr_fsr,
};

/* INDF register */

static uint8_t rd_indf(void)
{
    if (fsr == 0)
        return 0;
    return rd(fsr & 0x1f);
}

static void wr_indf(uint8_t value)
{
    if (fsr == 0)
        return;
    wr(fsr & 0x1f, value);
}

const struct sfr sfr_indf = {
    .rd = rd_indf,
    .wr = wr_indf,
};

/* PCL register */

static uint8_t rd_pcl(void)
{
    return get_pc() & 0xff;
}

static void wr_pcl(uint8_t value)
{
    /* not implemented */
}

const struct sfr sfr_pcl = {
    .rd = rd_pcl,
    .wr = wr_pcl,
};

/* STATUS register */

static uint8_t rd_status(void)
{
    return get_status();
}

static void wr_status(uint8_t value)
{
    /* not implemented */
}

const struct sfr sfr_status = {
    .rd = rd_status,
    .wr = wr_status,
};

/* OSCCAL register */

static uint8_t osccal;

static void init_osccal(void)
{
    osccal = 0xfe;
}

static uint8_t rd_osccal(void)
{
    return osccal;
}

static void wr_osccal(uint8_t value)
{
    osccal = value;
}

const struct sfr sfr_osccal = {
    .init = init_osccal,
    .rd = rd_osccal,
    .wr = wr_osccal,
};

/*
Definition of host machine dependent registers.
*/

const struct sfr sfr_gpio = {
    .init = init_gpio,
    .rd = rd_gpio,
    .wr = wr_gpio,
};

const struct sfr sfr_option = {
    .init = init_option,
    .rd = rd_option,
    .wr = wr_option,
};

const struct sfr sfr_tmr0 = {
    .init = init_tmr0,
    .rd = rd_tmr0,
    .wr = wr_tmr0,
};

const struct sfr sfr_trisgpio = {
    .init = init_trisgpio,
    .rd = rd_trisgpio,
    .wr = wr_trisgpio,
};

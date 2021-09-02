#include <stdint.h>

#include "gpio.h"
#include "option.h"
#include "tmr0.h"

#define OPTION_GPWU 0x80
#define OPTION_GPPU 0x40
#define OPTION_T0CS 0x20
#define OPTION_T0SE 0x10
#define OPTION_PSA 0x08
#define OPTION_PS 0x07

void init_option(void)
{

}

uint8_t rd_option(void)
{
    return 0xff;
}

void wr_option(uint8_t value)
{
    if (value & OPTION_GPPU)
        disable_weak_pullups();
    else
        enable_weak_pullups();

    if (value & OPTION_T0CS)
        ;
    else
        ;

    if (value & OPTION_T0SE)
        ;
    else
        ;

    if (value & OPTION_PSA) {

    }
    else {

    }
};

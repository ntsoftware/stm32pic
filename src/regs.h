#pragma once

void init_regs(void);
uint8_t rd(uint8_t reg);
void wr(uint8_t reg, uint8_t value);
void set_tris(uint8_t value);
void set_option(uint8_t value);

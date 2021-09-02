#pragma once

void init_gpio(void);
uint8_t rd_gpio(void);
void wr_gpio(uint8_t value);

void init_trisgpio(void);
uint8_t rd_trisgpio(void);
void wr_trisgpio(uint8_t value);

void enable_weak_pullups(void);
void disable_weak_pullups(void);

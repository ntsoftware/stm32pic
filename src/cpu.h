#pragma once

#define STATUS_C 0x01
#define STATUS_DC 0x02
#define STATUS_Z 0x04
#define STATUS_PD 0x08
#define STATUS_TO 0x10
#define STATUS_GPWUF 0x80

void init_cpu(void);
void step(void);
uint16_t get_pc(void);
uint8_t get_status(void);

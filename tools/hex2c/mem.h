#pragma once

#define MEM_WORDS 512

extern uint16_t mem[MEM_WORDS];

void init_mem(void);
void dump_mem(void);

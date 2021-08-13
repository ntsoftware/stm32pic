#pragma once

void init_instruction_set(void);
bool add_instruction(const char* name, uint16_t op, uint16_t op_mask);
int decode_instruction(uint16_t op);
size_t get_instruction_count(void);
const char* get_instruction_name(size_t index);

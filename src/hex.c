#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

#include "hex.h"

static struct {
    const char* hex;
    size_t n;
    size_t i;
    size_t line_start;
    size_t line_end;
    uint8_t* mem;
    size_t mem_size;
    uint32_t address;
    int eof;
} ctx;

static void init_context(const char* hex, size_t n, void* mem, size_t mem_size)
{
    ctx.hex = hex;
    ctx.n = n;
    ctx.i = 0;
    ctx.line_start = 0;
    ctx.line_end = 0;
    ctx.mem = mem;
    ctx.mem_size = mem_size;
    ctx.address = 0;
    ctx.eof = 0;
}

static int read_line(void)
{
    ctx.line_start = ctx.i;
    while (ctx.i < ctx.n && !isspace(ctx.hex[ctx.i]))
        ++ctx.i;
    ctx.line_end = ctx.i;
    while (ctx.i < ctx.n && isspace(ctx.hex[ctx.i]))
        ++ctx.i;
    return ctx.line_start != ctx.line_end;
}

static char get_start_code(void)
{
    if (ctx.line_start < ctx.line_end)
        return ctx.hex[ctx.line_start];
    else
        return 0;
}

static uint8_t nibble_to_byte(char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else
        return 0;
}

static uint8_t get_byte(int index)
{
    size_t i = ctx.line_start + 1 + (size_t)index * 2;
    if (i + 1 < ctx.line_end) {
        uint8_t msb = nibble_to_byte(ctx.hex[i]);
        uint8_t lsb = nibble_to_byte(ctx.hex[i + 1]);
        return (msb << 4) | lsb;
    }
    else {
        return 0;
    }
}

static int get_byte_count(void)
{
    return get_byte(0);
}

static uint32_t get_address(void)
{
    uint32_t msb = get_byte(1);
    uint32_t lsb = get_byte(2);
    return (msb << 8) | lsb;
}

static int get_record_type(void)
{
    return get_byte(3);
}

static int parse_data(void)
{
    int n = get_byte_count();
    uint32_t addr = ctx.address + get_address();
    for (int i = 0; i < n; ++i) {
        if (addr >= ctx.mem_size)
            return 0;
        uint8_t data = get_byte(i + 4);
        ctx.mem[addr] = data;
        ++addr;
    }
    return 1;
}

static int parse_end_of_file(void)
{
    ctx.eof = 1;
    return 1;
}

static int parse_extended_linear_address(void)
{
    if (get_byte_count() != 2)
        return 0;
    uint32_t msb = get_byte(4);
    uint32_t lsb = get_byte(5);
    ctx.address = (msb << 24) | (lsb << 16);
    return 1;
}

static int validate_line(void)
{
    if (get_start_code() != ':')
        return 0;
    int n = get_byte_count() + 5;
    uint8_t crc = 0;
    for (int i = 0; i < n; ++i)
        crc += get_byte(i);
    return crc == 0;
}

static int parse_line(void)
{
    int type = get_record_type();
    if (type == 0x00)
        return parse_data();
    else if (type == 0x01)
        return parse_end_of_file();
    else if (type == 0x04)
        return parse_extended_linear_address();
    return 0;
}

int parse_hex(const char* hex, size_t n, void* mem, size_t mem_size)
{
    init_context(hex, n, mem, mem_size);
    while (!ctx.eof && read_line()) {
        if (!validate_line())
            return -1;
        if (!parse_line())
            return -1;
    }
    return ctx.eof ? 0 : -1;
}

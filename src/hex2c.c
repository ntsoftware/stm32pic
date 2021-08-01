#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "hex.h"

#define MEM_WORDS 512

static void exit_with_error(int status, const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);
    exit(status);
}

static void print_usage(const char* name)
{
    printf("Usage: %s [OPTIONS] FILE\n", name);
    exit(0);
}

static struct {
    const char* file;
} args;

static void init_args(void)
{
    args.file = NULL;
}

static void parse_args(int argc, char* argv[])
{
    int pos = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
        }
        else {
            if (pos == 0)
                args.file = argv[i];
            else
                exit_with_error(1, "Unknown positional argument %s\n", argv[i]);
            ++pos;
        }
    }
    if (pos < 1)
        print_usage(argv[0]);
}

static size_t load_file(const char* filename, void* buf, size_t size)
{
    FILE* f = fopen(filename, "r");
    if (f == NULL)
        return 0;
    size_t n = fread(buf, 1, size, f);
    fclose(f);
    return n;
}

static uint16_t mem[MEM_WORDS];

static void init_mem(void)
{
    for (int i = 0; i < MEM_WORDS; ++i)
        mem[i] = 0x3fff;
}

static void dump_mem(void)
{
    for (int i = 0; i < MEM_WORDS; ++i) {
        if ((i % 8) == 0)
            printf("/* %04x */  ", i);
        printf("0x%04x,", mem[i]);
        if ((i % 8) == 7)
            putchar('\n');
        else
            putchar(' ');
    }
}

static char hex[64 * 1024];

int main(int argc, char* argv[])
{
    init_args();
    parse_args(argc, argv);
    init_mem();
    size_t n = load_file(argv[1], hex, sizeof(hex));
    if (n == 0)
        exit_with_error(1, "Error loading file %s\n", argv[1]);
    if (parse_hex(hex, n, mem, sizeof(mem)))
        exit_with_error(1, "Error parsing file %s\n", argv[1]);
    dump_mem();
    return 0;
}

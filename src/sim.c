#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "cpu.h"
#include "flash.h"
#include "ram.h"

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
    printf("Usage: %s [OPTIONS]\n", name);
    exit(0);
}

static struct {
    int dummy;
} args;

static void init_args(void)
{

}

static void parse_args(int argc, char* argv[])
{
    int pos = 0;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
        }
        else {
            exit_with_error(1, "Unknown positional argument %s\n", argv[i]);
            ++pos;
        }
    }
}

int main(int argc, char* argv[])
{
    init_args();
    parse_args(argc, argv);
    init_cpu();
    init_ram();
    init_flash();
    while (1) {
        step();
    }
    return 0;
}

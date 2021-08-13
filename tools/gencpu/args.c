#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "args.h"

struct args args;

void exit_with_error(int status, const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);
    exit(status);
}

static void print_usage(const char* name)
{
    printf("Usage: %s [OPTIONS] {lut|dec} FILE\n", name);
    exit(0);
}

static bool match_opt(const char* s, const char* short_opt, const char* long_opt)
{
    if (short_opt != NULL && strcmp(s, short_opt) == 0)
        return true;
    if (long_opt != NULL && strcmp(s, long_opt) == 0)
        return true;
    return false;
}

static enum cmd parse_cmd(const char* s)
{
    if (strcmp(s, "lut") == 0)
        return CMD_LUT;
    if (strcmp(s, "dec") == 0)
        return CMD_DEC;
    exit_with_error(1, "Unknown command \"%s\"", s);
}

static void set_default_args(void)
{
    args.file = NULL;
}

void parse_args(int argc, char* argv[])
{
    set_default_args();
    int pos = 0;
    for (int i = 1; i < argc; ++i) {
        if (match_opt(argv[i], "-h", "--help")) {
            print_usage(argv[0]);
        }
        else {
            if (pos == 0)
                args.cmd = parse_cmd(argv[i]);
            else if (pos == 1)
                args.file = argv[i];
            else
                exit_with_error(1, "Unknown positional argument %s\n", argv[i]);
            ++pos;
        }
    }
    if (pos < 2)
        print_usage(argv[0]);
}

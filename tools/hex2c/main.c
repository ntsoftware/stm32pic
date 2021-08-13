#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "args.h"
#include "hex.h"
#include "mem.h"

static size_t load_file(const char* filename, void* buf, size_t size)
{
    FILE* f = fopen(filename, "r");
    if (f == NULL)
        return 0;
    size_t n = fread(buf, 1, size, f);
    fclose(f);
    return n;
}

static char hex[64 * 1024];

int main(int argc, char* argv[])
{
    parse_args(argc, argv);
    init_mem();
    size_t n = load_file(args.file, hex, sizeof(hex));
    if (n == 0)
        exit_with_error(1, "Error loading file %s\n", args.file);
    if (!parse_hex(hex, n, mem, sizeof(mem)))
        exit_with_error(1, "Error parsing file %s\n", args.file);
    dump_mem();
    return 0;
}

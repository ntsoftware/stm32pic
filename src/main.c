#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "args.h"
#include "cpu.h"
#include "flash.h"
#include "regs.h"

int main(int argc, char* argv[])
{
    parse_args(argc, argv);
    init_cpu();
    init_regs();
    init_flash();
    while (1) {
        step();
    }
    return 0;
}

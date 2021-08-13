#pragma once

enum cmd {
    CMD_LUT,
    CMD_DEC,
    CMD_SWAP,
    CMD_BIT,
};

struct args {
    const char* file;
    enum cmd cmd;
};

extern struct args args;

void exit_with_error(int status, const char* msg, ...);
void parse_args(int argc, char* argv[]);

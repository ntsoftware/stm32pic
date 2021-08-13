#pragma once

struct args {
    const char* file;
};

extern struct args args;

void exit_with_error(int status, const char* msg, ...);
void parse_args(int argc, char* argv[]);

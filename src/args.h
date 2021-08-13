#pragma once

struct args {
    bool verbose;
};

extern struct args args;

void exit_with_error(int status, const char* msg, ...);
void parse_args(int argc, char* argv[]);

add_executable(hex2c)
target_sources(hex2c PRIVATE "main.c" "args.c" "args.h" "mem.c" "mem.h" "hex.c" "hex.h")
target_compile_features(hex2c PRIVATE c_std_99)
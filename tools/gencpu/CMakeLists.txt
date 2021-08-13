add_executable(gencpu)
target_sources(gencpu PRIVATE "main.c" "args.c" "args.h" "insfile.c" "insfile.h" "insset.c" "insset.h")
target_compile_features(gencpu PRIVATE c_std_99)
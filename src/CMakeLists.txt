add_executable(sim)
target_sources(sim PRIVATE "main.c" "cpu.c" "cpu.h" "flash.c" "flash.h" "regs.c" "regs.h" "args.c" "args.h" "swap_lut.h" "bit_lut.h")
target_compile_features(sim PRIVATE c_std_99)
target_include_directories(sim PRIVATE ${CMAKE_SOURCE_DIR}/programs/toggle)
add_executable(sim)
target_sources(sim PRIVATE sim.c cpu.c cpu.h ram.c ram.h flash.c flash.h)
target_compile_features(sim PRIVATE c_std_99)
target_include_directories(sim PRIVATE ${CMAKE_SOURCE_DIR}/programs/toggle)

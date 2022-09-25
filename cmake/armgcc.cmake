set(TOOLCHAIN_PREFIX arm-none-eabi)

set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_program(CMAKE_AR ${TOOLCHAIN_PREFIX}-gcc-ar)
find_program(CMAKE_GDB ${TOOLCHAIN_PREFIX}-gdb)
find_program(CMAKE_LD ${TOOLCHAIN_PREFIX}-ld)
find_program(CMAKE_LINKER ${TOOLCHAIN_PREFIX}-ld)
find_program(CMAKE_NM ${TOOLCHAIN_PREFIX}-gcc-nm)
find_program(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
find_program(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump)
find_program(CMAKE_RANLIB ${TOOLCHAIN_PREFIX}-gcc-ranlib)
find_program(CMAKE_STRIP ${TOOLCHAIN_PREFIX}-strip)
find_program(CMAKE_SIZE ${TOOLCHAIN_PREFIX}-size)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(cpu_flag "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS "${cpu_flag}")

set(CMAKE_ASM_FLAGS "${cpu_flag} -x assembler-with-cpp")

add_compile_options(
    -specs=nano.specs
    -Wl,--gc-sections
    -Wl,--cref
    -Wl,-print-memory-usage
)

function(create_hex exec)
    add_custom_command(
        TARGET ${exec}
        POST_BUILD
        COMMAND ${CMAKE_SIZE} ${CMAKE_BINARY_DIR}/${exec}.elf
        COMMAND ${CMAKE_OBJCOPY} -O binary -S ${CMAKE_BINARY_DIR}/${exec}.elf ${CMAKE_BINARY_DIR}/${exec}.bin
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_BINARY_DIR}/${exec}.elf ${CMAKE_BINARY_DIR}/${exec}.hex
        BYPRODUCTS ${CMAKE_BINARY_DIR}/${exec}.hex
    )
    
endfunction(create_hex)

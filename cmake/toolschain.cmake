set(triple arm-none-eabi)
set(CMAKE_LIBRARY_ARCHITECTURE ${triple})
set(TOOLCHAIN_PREFIX ${triple})

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_C_COMPILER_TARGET ${triple})

set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})

set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc)

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

set(cpu_flags "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS "${cpu_flags}")
set(CMAKE_ASM_FLAGS "${cpu_flags} -x assembler-with-cpp")
# set(CMAKE_ASM_FLAGS "-mcpu=cortex-m4 -g3 -x assembler-with-cpp --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb")

add_compile_options(
    -std=gnu11
    -g3
    -DUSE_HAL_DRIVER
    -DSTM32F411xE
    -DDEBUG
    -O0
    -ffunction-sections
    -fdata-sections
    -Wall
    -fstack-usage
    --specs=nano.specs
)

add_link_options(
    --specs=nosys.specs 
    -Wl,--gc-sections 
    -Wl,--start-group 
    -lc 
    -lm 
    -Wl,--end-group
)

function(create_hex exec)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND arm-none-eabi-size ${CMAKE_BINARY_DIR}/${target}.elf
        COMMAND arm-none-eabi-objcopy -O binary -S ${CMAKE_BINARY_DIR}/${target}.elf ${CMAKE_BINARY_DIR}/${target}.bin
        COMMAND arm-none-eabi-objcopy -O ihex ${CMAKE_BINARY_DIR}/${target}.elf ${CMAKE_BINARY_DIR}/${target}.hex
        BYPRODUCTS ${CMAKE_BINARY_DIR}/${target}.hex
    )
endfunction()




set(target trochilus)

file(GLOB_RECURSE srcs
    user_main.c
)

add_executable(${target} 
    ${srcs}
)

target_compile_definitions(${target} PUBLIC

)

get_property(libraries GLOBAL PROPERTY PROJECT_LIBRARIES)
get_property(libraries_interface GLOBAL PROPERTY PROJECT_LIBRARIES_INTERFACE)
get_property(link_script GLOBAL PROPERTY LINK_SCRIPT)

target_link_libraries(${target}
    ${libraries}
    ${libraries_interface}
    -Wl,-Map=${CMAKE_BINARY_DIR}/${target}.map
    -u_printf_float
    ${link_script}
)

create_hex(${target})
add_flash_target(${target})
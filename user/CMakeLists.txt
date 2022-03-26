set(target "trochilus")


file(GLOB APP_SOURCE_FILES
    user_main.c
)
add_executable(${target} 
    ${APP_SOURCE_FILES}
)


target_compile_definitions(${target} PUBLIC
    
)

target_include_directories(${target} PUBLIC
    
)


get_property(libraries GLOBAL PROPERTY PROJECT_LIBRARIES)
get_property(libraries_interface GLOBAL PROPERTY PROJECT_LIBRARIES_INTERFACE)
get_property(link_script GLOBAL PROPERTY LINK_SCRIPT)

MESSAGE(${libraries})
target_link_libraries(${target} 
    ${libraries}
    ${libraries_interface}
    -Wl,-Map=${CMAKE_BINARY_DIR}/${target}.map
    ${link_script}
    )

create_hex(${target})
add_flash_target(${target})

add_subdirectory(Drivers)
add_subdirectory(Core)
add_subdirectory(Middlewares)

set_property(GLOBAL PROPERTY LINK_SCRIPT
    -L${CMAKE_CURRENT_SOURCE_DIR}
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F411CEUx_FLASH.ld
)
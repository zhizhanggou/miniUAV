
# add_custom_target(erase_stlink
#     COMMAND st-flash erase
# )

# add_custom_target(reset_stlink
#     COMMAND st-flash reset
# )

# function(add_flash_target target)
#     add_custom_target(flash_${target}_stlink
#         COMMAND st-flash write ${CMAKE_BINARY_DIR}/${target}.hex"
#         DEPENDS ${target}
#     )
# endfunction(add_flash_target)
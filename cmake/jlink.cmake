set(JLINK_SCRIPT ${CMAKE_BINARY_DIR}/cmd.jlink)

add_custom_target(erase
    COMMAND echo "erase" > ${JLINK_SCRIPT}
    COMMAND echo "r" >> ${JLINK_SCRIPT}
    COMMAND echo "exit" >> ${JLINK_SCRIPT}
    COMMAND JLinkExe -if swd -device STM32F401CC -speed 4000 -CommanderScript ${JLINK_SCRIPT}
)

add_custom_target(reset
    COMMAND echo "r" > ${JLINK_SCRIPT}
    COMMAND echo "g" >> ${JLINK_SCRIPT}
    COMMAND echo "exit" >> ${JLINK_SCRIPT}
    COMMAND JLinkExe -if swd -device STM32F401CC -speed 4000 -CommanderScript ${JLINK_SCRIPT}
)

add_custom_target(halt
    COMMAND echo "h" > ${JLINK_SCRIPT}
    COMMAND echo "exit" >> ${JLINK_SCRIPT}
    COMMAND JLinkExe -if swd -device STM32F401CC -speed 4000 -CommanderScript ${JLINK_SCRIPT}
)

function(add_flash_target target)
    add_custom_target(flash_${target}
        COMMAND echo "r" > ${JLINK_SCRIPT}
        COMMAND echo "h" >> ${JLINK_SCRIPT}
        COMMAND echo "loadfile ${CMAKE_BINARY_DIR}/${target}.hex" >> ${JLINK_SCRIPT}
        COMMAND echo "r" >> ${JLINK_SCRIPT}
        COMMAND echo "g" >> ${JLINK_SCRIPT}
        COMMAND echo "exit" >> ${JLINK_SCRIPT}
        COMMAND JLinkExe -if swd -device STM32F401CC -speed 4000 -CommanderScript ${JLINK_SCRIPT}
        DEPENDS ${target}
    )
endfunction(add_flash_target)
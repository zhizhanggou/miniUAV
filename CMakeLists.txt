cmake_minimum_required(VERSION 3.10)
set(CMAKE_SYSTEM_NAME  "Generic")
project(trochilus LANGUAGES NONE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
# set(CMAKE_C_COMPILER arm-none-eabi-gcc)
# set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
# set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
# set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


include(jlink)
include(extensions)
include(toolschain)

enable_language(CXX C ASM)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(BUILD_SHARED_LIBS OFF)


set_property(GLOBAL PROPERTY PROJECT_LIBRARIES "")
set_property(GLOBAL PROPERTY PROJECT_LIBRARIES_INTERFACE "")
add_library(trochilus_global INTERFACE)
set_property(GLOBAL APPEND PROPERTY PROJECT_LIBRARIES trochilus_global)


add_subdirectory(boards)
add_subdirectory(rtos)
add_subdirectory(user)


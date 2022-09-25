cmake_minimum_required(VERSION 3.10)
set(CMAKE_SYSTEM_NAME  "Generic")
project(trochilus LANGUAGES NONE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(platform)
include(extensions)
include(programmer)
include(armgcc)

enable_language(CXX C ASM)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

set_property(GLOBAL PROPERTY PROJECT_SOURCES "")
set_property(GLOBAL PROPERTY PROJECT_LIBRARIES "")
set_property(GLOBAL PROPERTY PROJECT_LIBRARIES_INTERFACE "")

add_library(trochilus_global INTERFACE)
set_property(GLOBAL APPEND PROPERTY PROJECT_LIBRARIES trochilus_global)

add_subdirectory(Components)
add_subdirectory(boards)
add_subdirectory(User)
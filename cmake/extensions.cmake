#=============================================================================
#
#	px4_parse_function_args
#
#	This function simplifies usage of the cmake_parse_arguments module.
#	It is intended to be called by other functions.
#
#	Usage:
#		px4_parse_function_args(
#			NAME <name>
#			[ OPTIONS <list> ]
#			[ ONE_VALUE <list> ]
#			[ MULTI_VALUE <list> ]
#			REQUIRED <list>
#			ARGN <ARGN>)
#
#	Input:
#		NAME		: the name of the calling function
#		OPTIONS		: boolean flags
#		ONE_VALUE	: single value variables
#		MULTI_VALUE	: multi value variables
#		REQUIRED	: required arguments
#		ARGN		: the function input arguments, typically ${ARGN}
#
#	Output:
#		The function arguments corresponding to the following are set:
#		${OPTIONS}, ${ONE_VALUE}, ${MULTI_VALUE}
#
#	Example:
#		function test()
#			px4_parse_function_args(
#				NAME TEST
#				ONE_VALUE NAME
#				MULTI_VALUE LIST
#				REQUIRED NAME LIST
#				ARGN ${ARGN})
#			message(STATUS "name: ${NAME}")
#			message(STATUS "list: ${LIST}")
#		endfunction()
#
#		test(NAME "hello" LIST a b c)
#
#		OUTPUT:
#			name: hello
#			list: a b c
#
include(CMakeParseArguments)
function(ex_parse_function_args)

	cmake_parse_arguments(IN "" "NAME" "OPTIONS;ONE_VALUE;MULTI_VALUE;REQUIRED;ARGN" "${ARGN}")
	cmake_parse_arguments(OUT "${IN_OPTIONS}" "${IN_ONE_VALUE}" "${IN_MULTI_VALUE}" "${IN_ARGN}")

	if(OUT_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "${IN_NAME}: unparsed ${OUT_UNPARSED_ARGUMENTS}")
	endif()

	foreach(arg ${IN_REQUIRED})
		if (NOT OUT_${arg})
			if (NOT "${OUT_${arg}}" STREQUAL "0")
				message(FATAL_ERROR "${IN_NAME} requires argument ${arg}\nARGN: ${IN_ARGN}")
			endif()
		endif()
	endforeach()

	foreach(arg ${IN_OPTIONS} ${IN_ONE_VALUE} ${IN_MULTI_VALUE})
		set(${arg} ${OUT_${arg}} PARENT_SCOPE)
	endforeach()

endfunction()

#=============================================================================
#
#	px4_add_library
#
#	Like add_library but with PX4 platform dependencies
#

function(ex_add_library)
    ex_parse_function_args(
		NAME ex_add_library
		ONE_VALUE LIBNAME
		MULTI_VALUE COMPILE_FLAGS LINK_FLAGS SRCS INCLUDES DEPENDS
		REQUIRED LIBNAME
		ARGN ${ARGN})

    add_library(${LIBNAME} EXCLUDE_FROM_ALL ${SRCS})
    set_property(GLOBAL APPEND PROPERTY PROJECT_SOURCES ${SRCS})

    if(COMPILE_FLAGS)
        target_include_option(${LIBNAME} PRIVATE ${COMPILE_FLAGS})
    endif()

    if(INCLUDE)
        target_include_directories(${LIBNAME} PRIVATE ${INCLUDES})
    endif()

    target_include_directories(${LIBNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    if(DEPENDS)
    # using target_link_libraries for dependencies provides linking
    #  as well as interface include and libraries
        foreach(dep ${DEPENDS})
            get_target_property(dep_type ${dep} TYPE)
            if((${dep_type} STREQUAL "STATIC_LIBRARY") OR (${dep_type} STREQUAL "INTERFACE_LIBRARY"))
                target_link_libraries(${MODULE} PRIVATE ${dep})
            else()
                add_dependencies(${MODULE} ${dep})
            endif()
        endforeach()
    endif()

    target_link_libraries(${LIBNAME} PRIVATE trochilus_global)

    set_property(GLOBAL APPEND PROPERTY PROJECT_LIBRARIES ${LIBNAME})
    if(INCLUDES)
        target_include_directories(trochilus_global INTERFACE ${INCLUDES})
    endif()

    target_include_directories(trochilus_global INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

endfunction()


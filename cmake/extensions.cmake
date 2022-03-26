function(parse_function_args)

	cmake_parse_arguments(IN "" "NAME" "OPTIONS;ONE_VALUE;MULTI_VALUE;REQUIRED;ARGN" "${ARGN}")
	cmake_parse_arguments(OUT "${IN_OPTIONS}" "${IN_ONE_VALUE}" "${IN_MULTI_VALUE}" "${IN_ARGN}")

	if (OUT_UNPARSED_ARGUMENTS)
		#message(FATAL_ERROR "${IN_NAME}: unparsed ${OUT_UNPARSED_ARGUMENTS}")
		# TODO: reenable
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

function(ex_add_library)

    parse_function_args(    
        NAME ex_add_library
        ONE_VALUE LIBNAME
        MULTI_VALUE COMPILE_FLAGS LINK_FLAGS SRCS INCLUDES DEPENDS 
        ARGN ${ARGN})

    add_library(${LIBNAME} EXCLUDE_FROM_ALL ${SRCS})

    if(COMPILE_FLAGS)
        target_compile_options(${LIBNAME} PRIVATE ${COMPILE_FLAGS})
    endif()
    message(${INCLUDES})
    if(INCLUDES)
        target_include_directories(${LIBNAME} PRIVATE ${INCLUDES})
    endif()

    if(DEPENDS)
        foreach(dep ${DEPENDS})
            get_target_property(dep_type ${dep} TYPE)
            if(${dep_type} STREQUAL "STATIC_LIBRARY")
                target_link_libraries(${LIBNAME} PRIVATE ${dep})
            else()
                add_dependencies(${LIBNAME} ${dep})
            endif()
        endforeach()
    endif()

    target_link_libraries(${LIBNAME} PRIVATE trochilus_global)
    set_property(GLOBAL APPEND PROPERTY PROJECT_LIBRARIES ${LIBNAME})
    if(INCLUDES)
        target_include_directories(trochilus_global INTERFACE ${INCLUDES})
    endif()

    target_include_directories(trochilus_global INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
endfunction(ex_add_library)



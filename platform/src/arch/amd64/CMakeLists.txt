set(SRCS
    platform.c
)

if(MSVC)
    set(ASM_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/platform.asm"
    )

    list(APPEND SRCS ${ASM_SRCS})

    target_link_options(snplatform PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/platform.def")
endif()

set(USE_ABS_SRCS)
foreach(src IN LISTS SRCS)
    if(IS_ABSOLUTE "${src}")
        list(APPEND USE_ABS_SRCS "${src}")
    else()
        list(APPEND USE_ABS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    endif()
endforeach()
target_sources(snplatform PRIVATE ${USE_ABS_SRCS})

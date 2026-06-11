set(SRCS
    platform.c
)

if(MSVC)
    set(ASM_SRCS
        platform.asm
    )

    list(APPEND SRCS ${ASM_SRCS})

    target_link_options(snplatform PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/platform.def")
endif()

target_sources(snplatform PRIVATE ${SRCS})

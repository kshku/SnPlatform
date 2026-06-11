set(SRCS
    platform.c
)

if(MSVC AND CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64|aarch64|AARCH64")
    list(APPEND SRCS platform.asm)
endif()

target_sources(snplatform PRIVATE ${SRCS})

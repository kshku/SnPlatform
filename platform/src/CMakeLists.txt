set(HEADERFILES
    platform.h
    defines.h
    snplatform.h
)

set(SRCS
)

set(SPECIFIC_SRCS
    platform.c
)

set(SUBDIRS
    win32
    nix
)

set(INCLUDE_BASE "${PROJECT_SOURCE_DIR}/platform/include/snplatform")

list(TRANSFORM HEADERFILES PREPEND "${INCLUDE_BASE}/")

target_sources(snplatform PRIVATE ${HEADERFILES} ${SRCS})

foreach(dir IN LISTS SUBDIRS)
    foreach(src IN LISTS SPECIFIC_SRCS)
        target_sources(snplatform PRIVATE "${dir}/${src}")
    endforeach()
endforeach()

add_subdirectory(arch)

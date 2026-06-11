set(SUBDIRS
    amd64
    arm64
)

foreach(subdir IN LISTS SUBDIRS)
    add_subdirectory("${subdir}")
endforeach()

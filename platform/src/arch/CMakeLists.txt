set(SUBDIRS
    amd64
)

foreach(subdir IN LISTS SUBDIRS)
    add_subdirectory("${subdir}")
endforeach()

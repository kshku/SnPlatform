if(SN_PLATFORM_BUILD_SHARED)
    add_library(snplatform SHARED)
    target_compile_definitions(snplatform PRIVATE SN_EXPORT)
else()
    add_library(snplatform STATIC)
    target_compile_definitions(snplatform PUBLIC SN_PLATFORM_STATIC)
endif()

target_include_directories(snplatform PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(snplatform PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(snplatform PRIVATE sn_platform_configs)
target_link_libraries(snplatform PUBLIC sncore)

if(MSVC)
    add_custom_command(TARGET snplatform PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "$(IntDir)src/arch/amd64"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "$(IntDir)src/arch/arm64"
        COMMENT "Creating MASM output directories"
    )
endif()

add_subdirectory(src)

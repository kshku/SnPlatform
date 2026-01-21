set(SRCS
    test.c
)

add_executable(sn_platform_test)
target_sources(sn_platform_test PRIVATE ${SRCS})
target_link_libraries(sn_platform_test PRIVATE snplatform)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND SN_PLATFORM_BUILD_SHARED)
    add_custom_target(copy_dlls ALL
        COMMENT "Copy the dlls"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLLS:sn_platform_test>" "$<TARGET_FILE_DIR:sn_platform_test>"
        COMMAND_EXPAND_LISTS
    )

    add_dependencies(copy_dlls sn_platform_test)
endif()


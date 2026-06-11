target_sources(snplatform PRIVATE platform.c)

if(MSVC)
    set(ASM_SRC "${CMAKE_CURRENT_SOURCE_DIR}/platform.asm")
    set(ASM_OBJ "${CMAKE_CURRENT_BINARY_DIR}/platform.asm.obj")

    add_custom_command(
        OUTPUT "${ASM_OBJ}"
        COMMAND ml64.exe /c /nologo "/Fo${ASM_OBJ}" "/Ta${ASM_SRC}"
        MAIN_DEPENDENCY "${ASM_SRC}"
        COMMENT "Assembling platform.asm"
    )

    target_sources(snplatform PRIVATE "${ASM_OBJ}")
    target_link_options(snplatform PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/platform.def")
endif()

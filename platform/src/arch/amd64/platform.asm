.CODE

PUBLIC sn_cpuid 
PUBLIC sn_xgetbv
PUBLIC sn_rdtsc
PUBLIC sn_rdtscp

; eax, ebx, ecx, edx
sn_cpuid PROC
    ; Check whether eax param is null
    test rcx, rcx
    jz done

    ; rbx is used by cpuid
    push rbx

    ; Save rcx and rdx values in r10 and r11
    mov r10, rcx
    mov r11, rdx
    
    ; Move eax param value to eax reg
    mov eax, dword ptr [r10]

    ; Move ecx param value to ecx reg if exists
    mov ecx, 0
    test r8, r8
    jz do_cpuid
    mov ecx, dword ptr [r8]

do_cpuid:
    cpuid

    ; write_eax_param
    mov dword ptr[r10], eax

write_ebx_param:
    test r11, r11
    jz write_ecx_param
    mov dword ptr[r11], ebx

write_ecx_param:
    test r8, r8
    jz write_edx_param
    mov dword ptr[r8], ecx

write_edx_param:
    test r9, r9
    jz restore_regs
    mov dword ptr[r9], edx

restore_regs:
    pop rbx

done:
    ret
sn_cpuid ENDP

; ecx
sn_xgetbv PROC
    xgetbv
    shl rdx, 32
    or rax, rdx
    ret
sn_xgetbv ENDP

sn_rdtsc PROC
    rdtsc
    shl rdx, 32
    or rax, rdx
    ret
sn_rdtsc ENDP

sn_rdtscp PROC
    rdtscp
    shl rdx, 32
    or rax, rdx
    ret
sn_rdtscp ENDP

END

#pragma once

// Compiler
#if defined(__clang__)
    #define SN_COMPILER_CLANG
#elif defined(__GNUC__)
    #define SN_COMPILER_GCC
#elif defined(_MSC_VER)
    #define SN_COMPILER_MSVC
#else
    #error "Don't know whether works on this compiler!"
#endif

// Operating system
#if defined(__gnu_linux__) || defined(__linux__)
    #define SN_OS_LINUX
#elif defined(_WIN64)
    #define SN_OS_WINDOWS
#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
    #define SN_OS_MAC
#else
    #error "Don't know whether works on this OS!"
#endif

// Architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__) || defined(__amd64) || defined(__x86_64) || defined(_M_AMD64)
    #define SN_ARCH_AMD64 
#elif defined(__aarch64__)
    #define SN_ARCH_ARM64
#elif defined(__riscv) && (__riscv_xlen == 64)
    #define SN_ARCH_RISC64
#else
    #error "Couldn't detect the architecture"
#endif

// Endian
#if defined(__BYTE_ORDER)
    #if (__BYTE_ORDER == __BIG_ENDIAN)
        #define SN_ENDIAN_BIG
    #elif (__BYTE_ORDER == __LITTLE_ENDIAN)
        #define SN_ENDIAN_LITTLE
    #else
        #error "Couldn't detect the endian"
    #endif
#elif defined(__BYTE_ORDER__)
    #if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        #define SN_ENDIAN_BIG
    #elif (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        #define SN_ENDIAN_LITTLE
    #else
        #error "Couldn't detect the endian"
    #endif
#elif defined(__FLOAT_WORD_ORDER__)
    #if (__FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__)
        #define SN_ENDIAN_BIG
    #elif (__FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        #define SN_ENDIAN_LITTLE
    #else
        #error "Couldn't detect the endian"
    #endif
#elif defined(__BIG_ENDIAN__)
    #define SN_ENDIAN_BIG
#elif defined(__LITTLE_ENDIAN__)
    #define SN_ENDIAN_LITTLE
#elif defined(SN_OS_WINDOWS) // Windows is running on little endian everywhere
	#define SN_ENDIAN_LITTLE
#else
    #error "Couldn't detect the endian"
#endif


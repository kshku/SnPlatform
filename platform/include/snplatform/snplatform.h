#pragma once

#include "snplatform/platform.h"
#include "snplatform/defines.h"

#if defined(SN_PLATFORM_STATIC)
    #define SN_API
#else
    #ifdef SN_EXPORT
        #if defined(SN_OS_LINUX) || defined(SN_OS_MAC)
            #define SN_API __attribute__((visibility("default")))
        #elif defined(SN_OS_WINDOWS)
            #define SN_API __declspec(dllexport)
        #else
            #error "Should not reach here!"
        #endif
    #else
        #if defined(SN_OS_LINUX) || defined(SN_OS_MAC)
            #define SN_API
        #elif defined(SN_OS_WINDOWS)
            #define SN_API __declspec(dllimport)
        #else
            #error "Should not reach here!"
        #endif
    #endif
#endif

typedef enum snCPUVendor {
    SN_CPU_VENDOR_UNKNOWN = 0,
    SN_CPU_VENDOR_INTEL,
    SN_CPU_VENDOR_AMD,
    SN_CPU_VENDOR_ARM,
} snCPUVendor;

typedef enum snCPUFeature {
#if defined(SN_ARCH_AMD64)
    SN_CPU_FEATURE_SSE,
    SN_CPU_FEATURE_SSE2,
    SN_CPU_FEATURE_SSE3,
    SN_CPU_FEATURE_SSSE3,
    SN_CPU_FEATURE_SSE41,
    SN_CPU_FEATURE_SSE42,
    SN_CPU_FEATURE_AVX,
    SN_CPU_FEATURE_AVX2,
    SN_CPU_FEATURE_FMA,

    SN_CPU_FEATURE_CMPXCHG8B,
    SN_CPU_FEATURE_CMPXCHG16B,
    SN_CPU_FEATURE_RDRAND,
    SN_CPU_FEATURE_RDTSCP,
#elif defined(SN_ARCH_ARM64)
    SN_CPU_FEATURE_NEON,
    SN_CPU_FEATURE_CRC32,
    SN_CPU_FEATURE_ATOMICS,
#endif
    SN_CPU_FEATURE_MAX
} snCPUFeature;


/**
 * @brief Get the system memory page size.
 *
 * @return Returns the number of bytes.
 */
SN_API uint32_t sn_platform_page_size(void);

/**
 * @brief Get the L1 data cache line size.
 *
 * @note If fails to check the cache size, returns 0.
 *
 * @return Returns cache line size in bytes.
 */
SN_API uint32_t sn_platform_cache_line_size(void);

/**
 * @brief Get number of logical CPU cores.
 *
 * @return Returns number of logical CPU cores.
 */
SN_API uint32_t sn_platform_logical_core_count(void);

/**
 * @brief Get number of physical CPU cores.
 *
 * @note If fails to check physical core count, returns 0.
 *
 * @return Returns number of physical CPU cores.
 */
SN_API uint32_t sn_platform_physical_core_count(void);

/**
 * @brief Get the CPU vendor.
 *
 * @return Returns CPU vendor.
 */
SN_API snCPUVendor sn_platform_cpu_vendor(void);

/**
 * @brief Read raw CPU cycle or tick counter.
 *
 * @return Current cycle/tick count.
 */
SN_API uint64_t sn_platform_cpu_cycle_counter(void);

/**
 * @brief Check whether the CPU cycle counter is invariant.
 *
 * @return true if invariant across frequency changes, false otherwise.
 *
 * @note On x86, this corresponds to "Invariant TSC".
 */
SN_API bool sn_platform_cpu_cycle_counter_is_invariant(void);

/**
 * @brief Get cycle counter frequency.
 *
 * @return Frequency in Hz, or 0 if unknown or not calibrated.
 *
 * @note This value can be computed at runtime using calibration (using monotonic clock).
 */
SN_API uint64_t sn_platform_cpu_cycle_counter_frequency(void);

/**
 * @brief Check whether the CPU feature is available.
 *
 * @param feature The feature
 *
 * @return Returns true if available, false otherwise.
 */
SN_API bool sn_platform_cpu_feature_is_available(snCPUFeature feature);

#undef SN_API

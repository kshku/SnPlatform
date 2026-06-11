#include "snplatform/snplatform.h"

#if defined(SN_ARCH_ARM64)

    #if defined(SN_OS_LINUX)
        #include <sys/auxv.h>
        #include <asm/hwcap.h>
    #elif defined(SN_OS_MAC)
        #include <sys/sysctl.h>
    #elif defined(SN_OS_WINDOWS)
        #include <windows.h>
    #endif

uint64_t sn_cntvct_el0(void);

static void detect_features(uint8_t features[SN_CPU_FEATURE_MAX]);

SnCPUVendor sn_platform_cpu_vendor(void) {
    return SN_CPU_VENDOR_ARM;
}

uint64_t sn_platform_cpu_cycle_counter(void) {
    return sn_cntvct_el0();
}

bool sn_platform_cpu_cycle_counter_is_invariant(void) {
    return true;
}

uint64_t sn_platform_cpu_cycle_counter_frequency(void) {
    return 0;
}

bool sn_platform_cpu_feature_is_available(SnCPUFeature feature) {
    static uint8_t features[SN_CPU_FEATURE_MAX] = {0};
    static bool cached = false;

    if (!cached) {
        detect_features(features);
        cached = true;
    }

    return SN_BYTE_ARRAY_CHECK(features, feature);
}

static void detect_features(uint8_t features[SN_CPU_FEATURE_MAX]) {
    // NEON is mandatory in ARMv8-A
    SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_NEON);

    #if defined(SN_OS_LINUX)
    unsigned long hwcaps = getauxval(AT_HWCAP);
    if (hwcaps & HWCAP_CRC32)
        SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_CRC32);
    if (hwcaps & HWCAP_ATOMICS)
        SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_ATOMICS);
    if (hwcaps & HWCAP_AES)
        SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_AES);
    if (hwcaps & HWCAP_SHA1)
        SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SHA1);
    if (hwcaps & HWCAP_SHA2)
        SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SHA2);
    if (hwcaps & HWCAP_PMULL)
        SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_PMULL);
    #elif defined(SN_OS_MAC)
    {
        uint64_t val = 0;
        size_t size = sizeof(val);
        if (sysctlbyname("hw.optional.arm.FEAT_CRC32", &val, &size, NULL, 0) == 0 && val)
            SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_CRC32);
        val = 0;
        if (sysctlbyname("hw.optional.arm.FEAT_LSE", &val, &size, NULL, 0) == 0 && val)
            SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_ATOMICS);
        val = 0;
        if (sysctlbyname("hw.optional.arm.FEAT_AES", &val, &size, NULL, 0) == 0 && val)
            SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_AES);
        val = 0;
        if (sysctlbyname("hw.optional.arm.FEAT_SHA1", &val, &size, NULL, 0) == 0 && val)
            SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SHA1);
        val = 0;
        if (sysctlbyname("hw.optional.arm.FEAT_SHA256", &val, &size, NULL, 0) == 0 && val)
            SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SHA2);
        val = 0;
        if (sysctlbyname("hw.optional.arm.FEAT_PMULL", &val, &size, NULL, 0) == 0 && val)
            SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_PMULL);
    }
    #elif defined(SN_OS_WINDOWS)
    if (IsProcessorFeaturePresent(30))
        SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_CRC32);
    if (IsProcessorFeaturePresent(43))
        SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_ATOMICS);
    #else
    SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_CRC32);
    SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_ATOMICS);
    SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_AES);
    SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SHA1);
    SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SHA2);
    SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_PMULL);
    #endif
}

    #if !defined(SN_COMPILER_MSVC)

uint64_t sn_cntvct_el0(void) {
    uint64_t val;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r"(val));
    return val;
}

    #endif

#endif

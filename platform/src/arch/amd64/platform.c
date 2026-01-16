#include "snplatform/snplatform.h"

#if defined(SN_ARCH_AMD64)

/**
 * @brief Get the values of cpuid.
 *
 * To set the leaf and subleaf values set them in the parameters.
 * Can also pass NULL to parameters.
 *
 * Usage:
 * @code{.c}
 * uint32_t eax, ebx, ecx, edx;
 * eax = 7; // Set the leaf value (function value)
 * ecx = 0; // Set the subleaf value
 * sn_cpuid(&eax, &ebx, &ecx, NULL); // Ignoring edx value
 * sn_cpuid(&eax, &ebx, &ecx, &edx);
 * @endcode
 *
 * @param eax Pointer to store eax register value.
 * @param ebx Pointer to store ebx register value.
 * @param ecx Pointer to store ecx register value.
 * @param edx Pointer to store edx register value.
 */
void sn_cpuid(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

/**
 * @brief Get the 64 bit value set from xgetbv instruction.
 *
 * @param ecx Value of the ecx regiser to use.
 *
 * @return Returns the 64 bit value given by xgetbv instruction(edx:eax)
 */
uint64_t sn_xgetbv(uint32_t ecx);

/**
 * @brief rdtsc instruction.
 *
 * @return Returns the 64 bit value from instruction(edx:eax).
 */
uint64_t sn_rdtsc(void);

/**
 * @brief rdtscp instruction.
 *
 * @return Returns the 64 bit value from instruction(edx:eax).
 */
uint64_t sn_rdtscp(void);

static void detect_features(uint8_t features[SN_CPU_FEATURE_MAX]);

snCPUVendor sn_platform_cpu_vendor(void) {
    static snCPUVendor vendor = SN_CPU_VENDOR_UNKNOWN; // = 0
    if (vendor) return vendor;

    uint32_t eax, ebx, ecx, edx;
    eax = 0;
    sn_cpuid(&eax, &ebx, &ecx, &edx);
    
    if (ebx == 0x756e6547 && edx == 0x49656e69 && ecx == 0x6c65746e) vendor = SN_CPU_VENDOR_INTEL;
    else if (ebx == 0x68747541 && edx == 0x69746e65 && ecx == 0x444d4163) vendor = SN_CPU_VENDOR_AMD;
    else vendor = SN_CPU_VENDOR_UNKNOWN;
    
    return vendor;
}

uint64_t sn_platform_cpu_cycle_counter(void) {
    if (sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_RDTSCP))
        return sn_rdtscp();
    return sn_rdtsc();
}

bool sn_platform_cpu_cycle_counter_is_invariant(void) {
    static bool invariant = false;
    static bool checked = false;

    if (checked) return invariant;

    uint32_t eax, edx;
    eax = 0x80000007;
    sn_cpuid(&eax, NULL, NULL, &edx);

    invariant = SN_BIT_CHECK(edx, 8);
    checked = true;

    return invariant;
}

uint64_t sn_platform_cpu_cycle_counter_frequency(void) {
    return 0;
}

bool sn_platform_cpu_feature_is_available(snCPUFeature feature) {
    // Byte array for features
    static uint8_t features[SN_CPU_FEATURE_MAX] = {0};
    static bool cached = false;

    if (!cached) {
        detect_features(features);
        cached = true;
    }

    return SN_BYTE_ARRAY_CHECK(features, feature);
}

static void detect_features(uint8_t features[SN_CPU_FEATURE_MAX]) {
    uint32_t eax, ebx, ecx, edx;

    // Get the values for leaf = 1
    eax = 0x00000001;
    sn_cpuid(&eax, &ebx, &ecx, &edx);

    if (SN_BIT_CHECK(edx, 25)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SSE);

    if (SN_BIT_CHECK(edx, 26)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SSE2);

    if (SN_BIT_CHECK(ecx, 0)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SSE3);

    if (SN_BIT_CHECK(ecx, 9)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SSSE3);

    if (SN_BIT_CHECK(ecx, 19)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SSE41);

    if (SN_BIT_CHECK(ecx, 20)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_SSE42);

    if (SN_BIT_CHECK(ecx, 28 /* AVX */)) {
        if (SN_BIT_CHECK(ecx, 27 /* OSXSASN */)) {
            uint64_t xcr0 = sn_xgetbv(0);
            if ((xcr0 & 0x6) == 0x6) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_AVX);
        }
    }

    if (SN_BIT_CHECK(ecx, 12)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_FMA);

    if (SN_BIT_CHECK(edx, 8)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_CMPXCHG8B);

    if (SN_BIT_CHECK(ecx, 13)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_CMPXCHG16B);

    if (SN_BIT_CHECK(ecx, 30)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_RDRAND);

    // Get the values for leaf = 7, subleaf = 0
    eax = 0x00000007;
    ecx = 0x00000000;
    sn_cpuid(&eax, &ebx, &ecx, &edx);

    if (SN_BYTE_ARRAY_CHECK(features, SN_CPU_FEATURE_AVX) && SN_BIT_CHECK(ebx, 5))
        SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_AVX2);

    eax = 0x80000001;
    ecx = 0x00000000;
    sn_cpuid(&eax, &ebx, &ecx, &edx);

    if (!SN_BYTE_ARRAY_CHECK(features, SN_CPU_FEATURE_CMPXCHG8B))
        if (SN_BIT_CHECK(edx, 8)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_CMPXCHG8B);

    if (SN_BIT_CHECK(edx, 27)) SN_BYTE_ARRAY_SET(features, SN_CPU_FEATURE_RDTSCP);
}

#if !defined(SN_COMPILER_MSVC)

void sn_cpuid(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    if (!eax) return;

    uint32_t a = *eax, b, c = ecx ? *ecx : 0, d;

    __asm__ volatile("cpuid" : "+a"(a), "=b"(b), "+c"(c), "=d"(d));

    *eax = a;
    if (ebx) *ebx = b;
    if (ecx) *ecx = c;
    if (edx) *edx = d;
}

uint64_t sn_xgetbv(uint32_t ecx) {
    uint32_t eax, edx;
    __asm__ volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(ecx));
    return ((((uint64_t)edx) << 32) | (uint64_t)eax);
}

uint64_t sn_rdtsc(void) {
    uint32_t lo, hi;
    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return (((uint64_t)hi) << 32) | (uint64_t)lo;
}

uint64_t sn_rdtscp(void) {
    uint32_t lo, hi;
    __asm__ volatile("rdtscp" : "=a"(lo), "=d"(hi) :: "rcx");
    return (((uint64_t)hi) << 32) | (uint64_t)lo;
}

#endif

#endif

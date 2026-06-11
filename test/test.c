#define _GNU_SOURCE
#define TEST_ASSERT(x)                                                     \
    do {                                                                   \
        if (!(x)) {                                                        \
            fprintf(stderr, "FAIL [%s:%d]: %s\n", __FILE__, __LINE__, #x); \
            abort();                                                       \
        }                                                                  \
    } while (0)
#include <snplatform/snplatform.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void test_cpu_vendor(void) {
    SnCPUVendor v = sn_platform_cpu_vendor();
    TEST_ASSERT(v != SN_CPU_VENDOR_UNKNOWN);

    printf("[snPlatform] CPU vendor: %d\n", v);
}

static void test_cpu_features(void) {
#if defined(SN_ARCH_AMD64)
    TEST_ASSERT(sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE));
    TEST_ASSERT(sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE2));

    printf("[snPlatform] SSE    : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE));
    printf("[snPlatform] SSE2   : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE2));
    printf("[snPlatform] SSE3   : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE3));
    printf("[snPlatform] SSSE3  : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSSE3));
    printf("[snPlatform] SSE4.1 : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE41));
    printf("[snPlatform] SSE4.2 : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE42));
    printf("[snPlatform] AVX    : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_AVX));
    printf("[snPlatform] AVX2   : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_AVX2));
    printf("[snPlatform] FMA    : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_FMA));
    printf("[snPlatform] POPCNT : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_POPCNT));
    printf("[snPlatform] AESNI  : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_AESNI));
    printf("[snPlatform] MOVBE  : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_MOVBE));
    printf("[snPlatform] BMI1   : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_BMI1));
    printf("[snPlatform] BMI2   : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_BMI2));
    printf("[snPlatform] SHA    : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SHA));
    printf("[snPlatform] RDRAND : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_RDRAND));
    printf("[snPlatform] RDTSCP : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_RDTSCP));
#elif defined(SN_ARCH_ARM64)
    TEST_ASSERT(sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_NEON));
    printf("[snPlatform] NEON  : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_NEON));
    printf("[snPlatform] CRC32 : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_CRC32));
    printf("[snPlatform] AES   : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_AES));
    printf("[snPlatform] SHA1  : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SHA1));
    printf("[snPlatform] SHA2  : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SHA2));
    printf("[snPlatform] PMULL : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_PMULL));
    printf("[snPlatform] ATOM  : %d\n", sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_ATOMICS));
#endif
}

static void test_cycle_counter_frequency(void) {
    uint64_t freq = sn_platform_cpu_cycle_counter_frequency();

    if (freq == 0) {
        printf("[snPlatform] Cycle counter frequency: unknown (OK)\n");
        return;
    }

    TEST_ASSERT(freq > 100ULL * 1000 * 1000);
    TEST_ASSERT(freq < 10ULL * 1000 * 1000 * 1000);

    printf("[snPlatform] Cycle counter frequency: %llu Hz\n", (unsigned long long)freq);
}

static void test_cycle_counter_progress(void) {
    uint64_t t0 = sn_platform_cpu_cycle_counter();

#ifdef SN_OS_WINDOWS
    Sleep(10);
#else
    struct timespec sleep_time = {.tv_sec = 0, .tv_nsec = 10 * 1000000};
    nanosleep(&sleep_time, NULL);
#endif
    uint64_t t1 = sn_platform_cpu_cycle_counter();

    TEST_ASSERT(t1 > t0);

    printf("[snPlatform] Cycle counter progressed over time\n");
}

static void test_page_size(void) {
    uint32_t ps = sn_platform_page_size();
    TEST_ASSERT(ps > 0);
    TEST_ASSERT(ps <= 65536);

    printf("[snPlatform] Page size: %u\n", ps);
}

static void test_core_count(void) {
    uint32_t logical = sn_platform_logical_core_count();
    TEST_ASSERT(logical > 0);

    printf("[snPlatform] Logical cores: %u\n", logical);

    uint32_t physical = sn_platform_physical_core_count();

    printf("[snPlatform] Physical cores: %u\n", physical);
}

static void test_cache_line_size(void) {
    uint32_t cls = sn_platform_cache_line_size();
    TEST_ASSERT(cls > 0);
    TEST_ASSERT(cls <= 256);

    printf("[snPlatform] Cache line size: %u\n", cls);
}

int main(void) {
    printf("==== snPlatform CPU tests ====\n");

    test_cpu_vendor();
    test_cpu_features();
    test_cycle_counter_progress();
    test_cycle_counter_frequency();
    test_page_size();
    test_core_count();
    test_cache_line_size();

    printf("==== snPlatform CPU tests PASSED ====\n");
}

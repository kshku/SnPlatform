#define _GNU_SOURCE
#include <snplatform/snplatform.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

/* forward declarations if not public */
void sn_cpuid(uint32_t *eax, uint32_t *ebx,
              uint32_t *ecx, uint32_t *edx);
uint64_t sn_rdtsc(void);
uint64_t sn_rdtscp(void);

static void test_cpuid_basic(void)
{
    uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0;

    sn_cpuid(&eax, &ebx, &ecx, &edx);

    /* CPUID leaf 0 must return non-zero max leaf */
    assert(eax != 0);

    char vendor[13];
    ((uint32_t*)vendor)[0] = ebx;
    ((uint32_t*)vendor)[1] = edx;
    ((uint32_t*)vendor)[2] = ecx;
    vendor[12] = '\0';

    printf("[snPlatform] CPUID vendor string: %s\n", vendor);

    snCPUVendor v = sn_platform_cpu_vendor();
    assert(v != SN_CPU_VENDOR_UNKNOWN);
}

static void test_cpu_features(void)
{
#if defined(SN_ARCH_AMD64)
    /* These should be true on any remotely modern x86-64 CPU */
    assert(sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE));
    assert(sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE2));

    printf("[snPlatform] SSE  : %d\n",
           sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE));
    printf("[snPlatform] SSE2 : %d\n",
           sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_SSE2));
    printf("[snPlatform] AVX  : %d\n",
           sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_AVX));
#elif defined(SN_ARCH_ARM64)
    assert(sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_NEON));
    printf("[snPlatform] NEON : %d\n",
           sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_NEON));
#endif
}

static void test_rdtsc_monotonic(void)
{
    uint64_t a = sn_rdtsc();
    uint64_t b = sn_rdtsc();

    assert(b > a);

    printf("[snPlatform] RDTSC delta: %llu\n",
           (unsigned long long)(b - a));
}

static void test_rdtscp_ordering(void)
{
    uint64_t a = sn_rdtsc();
    uint64_t b = sn_rdtscp();

    assert(b >= a);

    printf("[snPlatform] RDTSCP >= RDTSC verified\n");
}

static void test_cycle_counter_frequency(void)
{
    uint64_t freq = sn_platform_cpu_cycle_counter_frequency();

    if (freq == 0) {
        printf("[snPlatform] Cycle counter frequency: unknown (OK)\n");
        return;
    }

    /* Sanity bounds: 100 MHz – 10 GHz */
    assert(freq > 100ULL * 1000 * 1000);
    assert(freq < 10ULL * 1000 * 1000 * 1000);

    printf("[snPlatform] Cycle counter frequency: %llu Hz\n",
           (unsigned long long)freq);
}

static void test_cycle_counter_progress(void)
{
    uint64_t t0 = sn_platform_cpu_cycle_counter();
    // 10ms
    struct timespec sleep_time = {
        .tv_sec = 0,
        .tv_nsec = 10 * 1000000
    };
    nanosleep(&sleep_time, NULL);
    uint64_t t1 = sn_platform_cpu_cycle_counter();

    assert(t1 > t0);

    printf("[snPlatform] Cycle counter progressed over time\n");
}

int main(void)
{
    printf("==== snPlatform CPU tests ====\n");

    test_cpuid_basic();
    test_cpu_features();
    test_rdtsc_monotonic();
    test_rdtscp_ordering();
    test_cycle_counter_progress();
    test_cycle_counter_frequency();

    printf("==== snPlatform CPU tests PASSED ====\n");
}


# SnPlatform

CPU feature detection and system information library written in C.

Detects CPU vendor, available instruction set extensions (SSE, AVX, etc.),
core counts, cache line size, page size, and provides a raw cycle counter.

## API

### Enums

```c
typedef enum SnCPUVendor {
    SN_CPU_VENDOR_UNKNOWN,
    SN_CPU_VENDOR_INTEL,
    SN_CPU_VENDOR_AMD,
    SN_CPU_VENDOR_ARM
} SnCPUVendor;

typedef enum SnCPUFeature {
    // x86-64
    SN_CPU_FEATURE_SSE, SN_CPU_FEATURE_SSE2, SN_CPU_FEATURE_SSE3,
    SN_CPU_FEATURE_SSSE3, SN_CPU_FEATURE_SSE41, SN_CPU_FEATURE_SSE42,
    SN_CPU_FEATURE_AVX, SN_CPU_FEATURE_AVX2, SN_CPU_FEATURE_FMA,
    SN_CPU_FEATURE_CMPXCHG8B, SN_CPU_FEATURE_CMPXCHG16B,
    SN_CPU_FEATURE_RDRAND, SN_CPU_FEATURE_RDTSCP,
    // ARM64
    SN_CPU_FEATURE_NEON, SN_CPU_FEATURE_CRC32, SN_CPU_FEATURE_ATOMICS,
    SN_CPU_FEATURE_MAX
} SnCPUFeature;
```

### Functions

| Function | Return | Description |
|----------|--------|-------------|
| `sn_platform_page_size()` | `uint32_t` | System memory page size in bytes |
| `sn_platform_cache_line_size()` | `uint32_t` | L1 data cache line size |
| `sn_platform_logical_core_count()` | `uint32_t` | Number of logical CPUs (hardware threads) |
| `sn_platform_physical_core_count()` | `uint32_t` | Number of physical CPU cores |
| `sn_platform_cpu_vendor()` | `SnCPUVendor` | CPU vendor (Intel, AMD, etc.) |
| `sn_platform_cpu_cycle_counter()` | `uint64_t` | Raw CPU cycle counter (RDTSC/P) |
| `sn_platform_cpu_cycle_counter_is_invariant()` | `bool` | True if TSC is invariant across frequency changes |
| `sn_platform_cpu_cycle_counter_frequency()` | `uint64_t` | TSC frequency in Hz (currently returns 0) |
| `sn_platform_cpu_feature_is_available(SnCPUFeature)` | `bool` | Check if a CPU feature is available |

## Usage

```c
#include <snplatform/snplatform.h>
#include <stdio.h>

int main(void) {
    printf("CPU vendor: %d\n", sn_platform_cpu_vendor());
    printf("Logical cores: %u\n", sn_platform_logical_core_count());
    printf("Page size: %u B\n", sn_platform_page_size());
    printf("Cache line: %u B\n", sn_platform_cache_line_size());

    if (sn_platform_cpu_feature_is_available(SN_CPU_FEATURE_AVX2))
        printf("AVX2 is available\n");

    uint64_t t0 = sn_platform_cpu_cycle_counter();
    // ... do some work ...
    uint64_t t1 = sn_platform_cpu_cycle_counter();
    printf("Cycle delta: %llu\n", (unsigned long long)(t1 - t0));

    return 0;
}
```

## Adding to your project

```cmake
include(FetchContent)
FetchContent_Declare(snplatform
    GIT_REPOSITORY https://github.com/kshku/SnPlatform.git
    GIT_TAG main
)
FetchContent_MakeAvailable(snplatform)

target_link_libraries(myapp PRIVATE snplatform)
```

## Build

```sh
cmake -B build
cmake --build build
```

| Option | Default | Description |
|--------|---------|-------------|
| `SN_PLATFORM_BUILD_SHARED` | `OFF` | Build as shared library |
| `SN_PLATFORM_BUILD_TEST` | `OFF` | Build tests |

## Platform Support

| Platform | x86-64 | ARM64 |
|----------|--------|-------|
| Linux | Full (CPUID, RDTSC) | Feature enum only |
| macOS | Full (CPUID, RDTSC) | Feature enum only |
| Windows | Full (CPUID, RDTSC via MASM) | — |

## Dependencies

- **SnCore** — fetched automatically via FetchContent

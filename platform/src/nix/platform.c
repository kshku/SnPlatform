#include "snplatform/snplatform.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

#include <unistd.h>
#if defined(SN_OS_MAC)
#include <sys/sysctl.h>
#endif

uint32_t sn_platform_cache_line_size(void) {
    static uint32_t cache_size = 0;
    if (cache_size) return cache_size;

#if defined(SN_OS_MAC)
    size_t size = sizeof(cache_size);
    if (sysctlbyname("hw.cachelinesize", &cache_size, &size, NULL, 0) != 0)
        cache_size = 0;
#else
    long v = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    if (v > 0) cache_size = (uint32_t)v;
#endif

    return cache_size;
}

uint32_t sn_platform_logical_core_count(void) {
    static uint32_t count = 0;
    if (count) return count;

    long v = sysconf(_SC_NPROCESSORS_ONLN);
    if (v > 0) count = (uint32_t)v;

    return count;
}

uint32_t sn_platform_physical_core_count(void) {
    static uint32_t cores = 0;
    if (cores) return cores;

#if defined(SN_OS_MAC)
    size_t size = sizeof(cores);
    if (sysctlbyname("hw.physicalcpu", &cores, &size, NULL, 0) != 0) cores = 0;
#else
    // No solution yet.
#endif

    return cores;
}

uint32_t sn_platform_page_size(void) {
    static uint32_t page_size;
    if (page_size) return page_size;

    long ps = sysconf(_SC_PAGE_SIZE);
    page_size = SN_MAX(0, ps);

    SN_ASSERT(page_size > 0);
    return page_size;
}

#endif

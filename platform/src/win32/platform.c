#include "snplatform/snplatform.h"

#if defined(SN_OS_WINDOWS)

#include <windows.h>

uint32_t sn_platform_cache_line_size(void) {
    static uint32_t cache_size = 0;
    if (cache_size) return cache_size;

    SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX buffer[64] = {0};
    DWORD len = SN_ARRAY_LENGTH(buffer) * sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
    if (GetLogicalProcessorInformationEx(RelationCache, buffer, &len)) {
        uint8_t *ptr = (uint8_t *)&buffer;
        uint8_t *end = ptr + len;

        while (ptr < end) {
            SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *entry =
                (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *)ptr;
            if (entry->Cache.Level == 1 && entry.Cache.Type == CacheData) {
                cache_size = entry->Cache.LineSize;
                break;
            }
            ptr += entry->Size;
        }
    }

    return cache_size;
}

uint32_t sn_platform_logical_core_count(void) {
    static uint32_t count = 0;
    if (count) return count;

    count = (uint32_t)GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);

    return count;
}

uint32_t sn_platform_physical_core_count(void) {
    static uint32_t cores = 0;
    if (cores) return cores;

    SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX buffer[64] = {0};
    DWORD len = SN_ARRAY_LENGTH(buffer) * sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX);
    if (GetLogicalProcessorInformationEx(RelationCache, buffer, &len)) {
        uint8_t *ptr = (uint8_t *)&buffer;
        uint8_t *end = ptr + len;

        while (ptr < end) {
            SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *entry =
                (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *)ptr;
                        if (entry->Relationship == RelationProcessorCore)
                cores++;
            ptr += entry->Size;
        }
    }

    return cores;
}

uint32_t sn_platform_page_size(void) {
    static uint32_t page_size = 0;
    if (page_size) return page_size;

    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    page_size = (uint32_t)system_info.dwPageSize;

    SN_ASSERT(page_size > 0);
    return page_size;
}

#endif

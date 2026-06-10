#pragma once

#include <sncore/api_common.h>

#if defined(SN_PLATFORM_STATIC)
    #define SN_PLATFORM_API
#elif defined(SN_EXPORT)
    #define SN_PLATFORM_API SN_API_HELPER_EXPORT
#else
    #define SN_PLATFORM_API SN_API_HELPER_IMPORT
#endif

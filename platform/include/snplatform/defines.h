#pragma once

#include "snplatform/platform.h"

#include <stdalign.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#if defined(SN_COMPILER_MSVC)
    #define SN_FORCE_INLINE static __forceinline
#else
    #define SN_FORCE_INLINE static inline __attribute__((always_inline))
#endif

#define SN_INLINE static inline

#define SN_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)

#define SN_ASSERT(x) assert(x)

#define SN_SHOULD_NOT_REACH_HERE (SN_ASSERT(false))

#define SN_UNUSED(x) (void)(x)

#define SN_ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

#define SN_STRINGIFY(x) #x
#define SN_STRINGIFY_EXPANDED(x) SN_STRINGIFY(x)

#define SN_CONCAT(x, y) x##y
#define SN_CONCAT_EXPANDED(x, y) SN_CONCAT(x, y)
#define SN_CONCAT3(x, y, z) x##y##z
#define SN_CONCAT_EXPANDED3(x, y, z) SN_CONCAT3(x, y, z)

#define SN_BIT_FLAG(n) (1 << (n))
#define SN_BIT_SET(x, n) ((x) |= SN_BIT_FLAG(n))
#define SN_BIT_CLEAR(x, n) ((x) &= ~SN_BIT_FLAG(n))
#define SN_BIT_TOGGLE(x, n) ((x) ^= SN_BIT_FLAG(n))
#define SN_BIT_CHECK(x, n) ((x) & SN_BIT_FLAG(n))
#define SN_BIT_SET_VALUE(x, n) ((x) | SN_BIT_FLAG(n))
#define SN_BIT_CLEARED_VALUE(x, n) ((x) & ~SN_BIT_FLAG(n))
#define SN_BIT_TOGGLED_VALUE(x, n) ((x) ^ SN_BIT_FLAG(n))

#define SN_BYTE_ARRAY_SET(arr, n) (((uint8_t *)arr)[(n) >> 3] |= SN_BIT_FLAG((n) & 0x7))
#define SN_BYTE_ARRAY_CLEAR(arr, n) (((uint8_t *)arr)[(n) >> 3] &= ~SN_BIT_FLAG((n) & 0x7))
#define SN_BYTE_ARRAY_TOGGLE(arr, n) (((uint8_t *)arr)[(n) >> 3] ^= SN_BIT_FLAG((n) & 0x7))
#define SN_BYTE_ARRAY_CHECK(arr, n) (((uint8_t *)arr)[(n) >> 3] & SN_BIT_FLAG((n) & 0x7))

#define SN_MAX(a, b) ((a) > (b) ? (a) : (b))

#define SN_MIN(a, b) ((a) < (b) ? (a) : (b))

#define SN_CLAMP(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))

#define SN_IS_POWER_OF_TWO(x) (((((uint64_t)(x)) - 1) & ((uint64_t)(x))) == 0)

#define SN_IS_ALIGNED(x, align) ((((uint64_t)(x)) & ((align) - 1)) == 0)
#define SN_GET_ALIGNED(x, align) ((((uint64_t)(x)) + (align) - 1) & ~((align) - 1))
#define SN_GET_ALIGNED_PTR(x, type) ((type *)SN_GET_ALIGNED((x), alignof(type)))
#define SN_GET_NEXT_ALIGNED(x, align) ((((uint64_t)(x)) + (align)) & ~((align) - 1))
#define SN_GET_NEXT_ALIGNED_PTR(x, align) ((type *)SN_GET_NEXT_ALIGNED((x), alignof(type)))

#define SN_PTR_DIFF(a, b) (((uint64_t)(a)) - ((uint64_t)(b)))


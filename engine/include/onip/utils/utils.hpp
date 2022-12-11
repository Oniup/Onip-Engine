#ifndef __ONIP_TYPES_TYPES_HPP__
#define __ONIP_TYPES_TYPES_HPP__

#include <iostream>
#include <cassert>
#include <cmath>

#ifndef ONIP_NO_FORCE_INLINE
    #define ONIP_INLINE __forceinline
#else
    #define ONIP_INLINE inline
#endif // ONIP_NO_FORCE_INLINE

#ifndef NDEBUG
    #define ONIP_ASSERT_FMT(condition, format, ...) {\
        if (!condition) {\
            printf(format, __VA_ARGS__);\
            exit(-1);\
        }\
    }\

#else
    #define ONIP_ASSERT_FMT(condition, format, ...)

#endif // NDEBUG 

#define ONIP_LOG(condition, format, ...) // TODO: implement logging system in utils first ...

namespace onip {
    using UUID = uint64_t;
    using Byte = char;

    struct Utils {
        template <typename _T>
        static const _T& max(const _T& a, const _T& b) {
            return a > b ? a : b;
        }

        template <typename _T>
        static const _T& min(const _T& a, const _T& b) {
            return a > b ? b : a;
        }

        static int rand_int32();
        static uint32_t randUint32();
        static uint64_t randUint64();
    };
}


#endif // __ONIP_TYPES_TYPES_HPP__
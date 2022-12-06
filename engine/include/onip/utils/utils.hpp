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
    #define onipAssert(condition, format, ...) {\
        if (!condition) {\
            printf(format, __VA_ARGS__);\
            exit(-1);\
        }\
    }\

#else
    #define onipAssert(condition, format, ...)

#endif // NDEBUG 

#define ONIP_LOG(condition, format, ...) // TODO: implement logging system in utils first ...

namespace onip {
    typedef uint64_t UUID;
    typedef char Byte;

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
        static uint32_t rand_uint32();
        static uint64_t rand_uint64();
    };
}


#endif // __ONIP_TYPES_TYPES_HPP__
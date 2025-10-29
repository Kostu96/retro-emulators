#pragma once

#if defined(__GNUC__) || defined(__clang__)
#define BEGIN_ALLOW_ANON_STRUCTS \
        _Pragma("GCC diagnostic push") \
        _Pragma("GCC diagnostic ignored \"-Wpedantic\"")
#define END_ALLOW_ANON_STRUCTS \
        _Pragma("GCC diagnostic pop")
#else
#define BEGIN_ALLOW_ANON_STRUCTS
#define END_ALLOW_ANON_STRUCTS
#endif

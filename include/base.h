// base.h - mini C header i use everywhere
// public domain'd
// https://tryh4rd.dev/

#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#ifndef DEFAULT_ERROR_STRUCT
#    define DEFAULT_ERROR_STRUCT

typedef struct Error {
    const char* msg;
} Error;
#endif

#ifndef DEFAULT_RESULT_STRUCT
#    define DEFAULT_RESULT_STRUCT

typedef struct Result {
    bool ok;
    u64 code;
    Error err;
} Result;
#endif

#define ERROR_INIT(m) \
    (Error) {         \
        .msg = m      \
    }
#define RESULT_INIT(o, c, e)         \
    (Result) {                       \
        .ok = o, .code = c, .err = e \
    }

/* gcc/clang extension macros (fun!) */
#ifndef DISABLE_COMPILER_EXTENSIONS
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define cleanup(function, declaration) __attribute__((cleanup(function)))declaration
#define packed(name) __attribute__((packed))name
#define unreachable __builtin_unreachable()
#define panick __builtin_trap()

#define assert(x) do { if (unlikely(!(x))) panick; } while (0)
#endif

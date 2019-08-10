#ifndef _MOS6502_COMMON_H_
#define _MOS6502_COMMON_H_

#include <stdint.h>
#include <stdio.h>

#define internal static

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;

#define DEBUG

#ifdef DEBUG
#define Assert(X) if(!(X)){ fprintf(stderr, "Assertion failed! line: %d\n", __LINE__); fflush(stderr); __debugbreak();}
#define DPRINT(F, ...) fprintf(stderr, (F), __VA_ARGS__)
#else
#define Assert(X)
#define DPRINT
#endif

#define UNIMPLEMENTED __debugbreak()

#define KB(X) ((X)*1024)

#endif
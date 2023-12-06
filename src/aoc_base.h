#ifndef AOC_BASE_H
#define AOC_BASE_H

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef uint32_t u32;
typedef int64_t s64;
typedef __int128 s128;
typedef float f32;

#define S32_MAX 0x7fffffff
#define S64_MAX 0x7fffffffffffffff

#endif
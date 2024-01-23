#ifndef STRARR_H
#define STRARR_H

#include "aoc.h"

typedef struct {
    str_t* arr;
    size_t size;
    size_t capacity;
} strarr_t;

void strarr_push(strarr_t* arr, str_t str);
void strarr_free(strarr_t* arr);
strarr_t str_split_char(str_t str, char delim);

#endif
#ifndef AOC_INT_ARR_H
#define AOC_INT_ARR_H

#include "aoc_base.h"

struct str_t;
typedef struct str_t str_t;

typedef struct {
    s64* data;
    size_t capacity;
    size_t size;
    bool owns_data;
} int_arr_t;

int_arr_t int_arr_create();
int_arr_t int_arr_create_with_capacity(int);
int_arr_t int_arr_from_str(str_t str);
int_arr_t int_arr_ref(int_arr_t arr);
void int_arr_free(int_arr_t);
void int_arr_push(int_arr_t* arr, s64 value); 
void int_arr_clear(int_arr_t* arr);
s64 int_arr_at(int_arr_t arr, int idx);
void int_arr_insert_sorted(int_arr_t* arr, s64 value);
void int_arr_print(int_arr_t* l);
bool int_arr_sorted_contains(int_arr_t* arr, s64 value);
static inline s64 int_arr_back(int_arr_t arr) {return arr.size == 0 ? 0 : arr.data[arr.size - 1];}
static inline s64 int_arr_front(int_arr_t arr) {return arr.size == 0 ? 0 : arr.data[0];}
s64 int_arr_pop_front(int_arr_t* arr);
s64 int_arr_sum(int_arr_t arr);
s64 int_arr_max(int_arr_t arr);
#endif
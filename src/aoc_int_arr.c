#include "aoc_int_arr.h"
#include "aoc.h"
#include <stdio.h>
#include <stdbool.h>

int_arr_t int_arr_create_with_capacity(int capacity) {
    return (int_arr_t){ 
        .data=malloc(capacity*sizeof(s64)),
        .capacity=capacity,
        .owns_data=true,
        .size=0};
}

int_arr_t int_arr_create() {
    return int_arr_create_with_capacity(16);
}

int_arr_t int_arr_from_str(str_t str) {
    str_iter_t iter = str_iter_begin(&str);
    int_arr_t arr = str_iter_match_int_list(&iter);
    return arr;
}

int_arr_t int_arr_ref(int_arr_t arr) {
    return (int_arr_t) {
        .data=arr.data,
        .capacity=0,
        .owns_data=false,
        .size=arr.size
    };
}

void int_arr_free(int_arr_t l) {
    free(l.data);
}

void int_arr_push(int_arr_t* arr, s64 value) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(s64));
    }
    arr->data[arr->size++] = value;
}

void int_arr_insert_sorted(int_arr_t* arr, s64 value) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(s64));
    }

    int i = arr->size;
    while (i > 0) {
        if (arr->data[i-1] > value) arr->data[i] = arr->data[i-1];
        else break;
        --i;
    }
    arr->data[i] = value;
    arr->size++;
}

void int_arr_print(int_arr_t* l) {
    for (int i = 0; i < l->size; ++i) {
        printf("%ld ", l->data[i]);
    }
    printf("\n");
}

void int_arr_clear(int_arr_t* arr) { arr->size = 0; }

s64 int_arr_at(int_arr_t arr, int idx) {
    assert(idx < arr.size);
    return arr.data[idx];
}

bool int_arr_sorted_contains(int_arr_t* arr, s64 val) {
    for (int i = 0; i < arr->size; ++i) {
        s64 arr_val = arr->data[i];
        if (arr_val == val) return true;
        else if (arr_val > val) return false;
    }
    return false;
}

s64 int_arr_pop_front(int_arr_t* arr) {
    if (arr->size == 0) return 0;

    s64 res = *arr->data;

    if (arr->owns_data) {
        memmove(arr->data, arr->data + 1, arr->size - 1);
        arr->size--;
    } else {
        arr->data++;
        arr->size--;
    }    
    return res;
}

s64 int_arr_sum(int_arr_t arr) {
    s64 sum = 0;
    for (int i = 0; i < arr.size; ++i) sum+=arr.data[i];
    return sum;
}

s64 int_arr_max(int_arr_t arr) {
    s64 res = -S32_MAX;
    for (int i = 0; i < arr.size; ++i) res = max_s64(res, arr.data[i]);
    return res;
}

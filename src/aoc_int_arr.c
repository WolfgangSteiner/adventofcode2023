#include "aoc_int_arr.h"
#include <stdio.h>
#include <stdbool.h>

int_arr_t int_arr_create() {
    return (int_arr_t){ 
        .data=malloc(1024*sizeof(int)),
        .capacity=1024,
        .size=0};
}

void int_arr_free(int_arr_t l) {
    free(l.data);
}

void int_arr_push(int_arr_t* arr, s64 value) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(int));
    }
    arr->data[arr->size++] = value;
}

void int_arr_insert_sorted(int_arr_t* arr, s64 value) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(int));
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
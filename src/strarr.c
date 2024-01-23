#include "strarr.h"


void strarr_push(strarr_t* arr, str_t str) {
    if (arr->arr == 0) {
        arr->capacity = 16;
        arr->arr = calloc(arr->capacity, sizeof(str_t));
    } else if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->arr = realloc(arr->arr, arr->capacity * sizeof(str_t));
    }
    arr->arr[arr->size++] = str;
}

void strarr_free(strarr_t* arr) {
    for (int i = 0; i < arr->size; ++i) str_free(arr->arr[i]);
    if (arr->arr) free(arr->arr);
    arr->arr = 0;
}

strarr_t str_split_char(str_t str, char delim) {
    strarr_t res = {0};
    str_iter_t iter = str_iter_begin(&str);
    while (!str_iter_is_end(&iter)) {
        strarr_push(&res, str_iter_match_up_to_char(&iter, delim));
        str_iter_inc(&iter);
    }
    return res;
}
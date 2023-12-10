#include "aoc.h"

char* day = "09";
s64 part_one_test_value = 114;
s64 part_two_test_value = 2;

bool int_arr_all(int_arr_t arr, s64 value) {
    for (int i = 0; i < arr.size; ++i) {
        if (arr.data[i] != value) return false;
    }
    return true;
}

int_arr_t diff(int_arr_t arr) {
    int_arr_t d = int_arr_create();
    for (int i = 1; i < arr.size; ++i) {
        int_arr_push(&d, arr.data[i] - arr.data[i-1]);
    }
    return d;
}

s64 extrapolate_value(int_arr_t arr) {
    int_arr_t d  = diff(arr);
    s64 diff_val = 0;
    if (!int_arr_all(d, 0)) {
        diff_val = extrapolate_value(d);
    } 
    int_arr_free(d);
    return diff_val + int_arr_back(arr);
}

s64 extrapolate_first_value(int_arr_t arr) {
    int_arr_t d  = diff(arr);
    s64 diff_val = 0;
    if (!int_arr_all(d, 0)) {
        diff_val = extrapolate_first_value(d);
    } 
    int_arr_free(d);
    return int_arr_front(arr) - diff_val;
}

s64 apply_func(str_t input, s64(func)(int_arr_t arr)) {
    str_iter_t iter = str_iter_begin(&input);
    s64 sum = 0;
    while (!str_iter_is_end(&iter)) {
        int_arr_t arr = str_iter_match_int_list(&iter);
        sum += func(arr);
        int_arr_free(arr);
        str_iter_match_newline(&iter);
    }

    return sum;
}


s64 part_one(str_t input) {
    return apply_func(input, extrapolate_value);
}

s64 part_two(str_t input) {
    return apply_func(input, extrapolate_first_value);
}

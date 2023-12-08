#include "aoc.h"
#include <time.h>

typedef struct {
    double start_time, end_time;
} timer_t;

timer_t timer_start() {
    timer_t t = {0};
    t.start_time = (double)clock() / CLOCKS_PER_SEC;
    return t;
}

double timer_end(timer_t t) {
    t.end_time = (double)clock() / CLOCKS_PER_SEC;
    return (t.end_time - t.start_time) * 1000.0;
}

str_t get_input_file_name(char* day) {
    str_t res = str_new("input/");
    str_append(&res, str_ref(day));
    str_append(&res, str_ref(".txt"));
    return res;
}

str_t get_part_one_test_file_name(char* day) {
    str_t res = str_new("input/");
    str_append(&res, str_ref(day));
    str_append(&res, str_ref("a.txt"));
    return res;
}

str_t get_part_two_test_file_name(char* day) {
    str_t res = str_new("input/");
    str_append(&res, str_ref(day));
    str_append(&res, str_ref("b.txt"));
    if (!file_exists(res)) {
        return get_part_one_test_file_name(day);
    }
    return res;
}

extern char* day;
extern s64 part_one_test_value, part_two_test_value;
extern s64 part_one(str_t);
extern s64 part_two(str_t);

void check_part_one() {
    str_t file_name = get_part_one_test_file_name(day);
    str_t data = read_file(file_name);
    s64 res = part_one(data);
    printf("test:     ");
    if (res != part_one_test_value) {
        printf("failed: expected %ld but got %ld\n", part_one_test_value, res);
    } else {
        printf("pass\n");
    }
    str_free(file_name);
    str_free(data);
}

void check_part_two() {
    str_t file_name = get_part_two_test_file_name(day);
    str_t data = read_file(file_name);
    s64 res = part_two(data);
    printf("test:     ");
    if (res != part_two_test_value) {
        printf("failed: expected %ld but got %ld\n", part_two_test_value, res);
    } else {
        printf("pass\n");
    }
    str_free(file_name);
    str_free(data);
}

void compute_part_one(str_t data) {
    timer_t t = timer_start();
    s64 res = part_one(data);
    double elapsed = timer_end(t);
    printf("solution: %ld\n", res);
    printf("time:     %.3f ms", elapsed);
    printf("\n\n");
}

void compute_part_two(str_t data) {
    timer_t t = timer_start();
    s64 res = part_two(data);
    double elapsed = timer_end(t);
    printf("solution: %ld\n", res);
    printf("time:     %.3f ms", elapsed);
    printf("\n\n");
}

int main(int, char**) {
    assert(is_digit(day[0]));
    str_t file_name = get_input_file_name(day);
    str_t data = read_file(file_name);
    printf("\n");
    printf("Part one:\n");
    printf("=====================\n");
    check_part_one();
    compute_part_one(data);
    printf("\n");
    printf("Part two:\n");
    printf("=====================\n");
    check_part_two();
    compute_part_two(data);
    str_free(file_name);
    str_free(data);    
}
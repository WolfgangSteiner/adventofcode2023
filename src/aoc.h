#ifndef AOC_H
#define AOC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "aoc_int_arr.h"
#include "aoc_base.h"

typedef struct {
    char* data;
    size_t size;
    bool is_valid;
    bool owns_data; } str_t;

typedef struct {
    str_t* str;
    int pos; } str_iter_t;

typedef struct {
    char* data;
    size_t capacity;
    int width, height; } grid_t;

str_t str_copy(str_t str);

static inline s64 min_s64(s64 a, s64 b) { return a < b ? a : b; }
static inline s64 max_s64(s64 a, s64 b) { return a > b ? a : b; }
static inline size_t min_size_t(size_t a, size_t b) { return a < b ? a : b; }
static inline size_t max_size_t(size_t a, size_t b) { return a > b ? a : b; }


int str_to_int(str_t str);
s64 str_to_s64(str_t str);
char str_at(str_t str, int idx);
char str_get_char(str_t str, size_t pos);
bool str_contains(str_t, str_t);
bool str_contains_char(str_t, char);
// create new str by referencing cstr
// new str does not own memory
str_t str_ref(char* cstr);
str_t str_substr(str_t, int, int);

// create new str by copying cstr data
str_t str_new(char* cstr);
str_t str_new_with_capacity(size_t capacity);
void str_free(str_t);

str_t str_cat(str_t a, str_t b);

void str_append(str_t* str, str_t append_str);
void str_append_space(str_t* str);
void str_append_char(str_t* str, char c);
char* str_cstr(str_t str);
char* str_copy_to_cstr(str_t str);

str_t str_lstrip(str_t);

typedef str_t(*str_format_callback_t)(va_list*, str_t pattern);
str_t str_format(str_t fmt, ...); 
void str_format_register_pattern(str_t pattern, str_format_callback_t callback);

str_iter_t str_find_char(str_t str, char c);
str_t str_split_tail_at_char(str_t str, char c);
bool str_eq(str_t a, str_t b);

bool str_iter_is_end(str_iter_t* iter);
bool str_iter_is_rend(str_iter_t* iter);
str_iter_t str_iter_begin(str_t* str);
str_iter_t str_iter_rbegin(str_t* str);
void str_print(str_t str);
str_t get_line(str_iter_t* iter);
str_t read_file(str_t file_name);
bool file_exists(str_t file_name);

int char_to_int(char c);
bool is_digit(char c);
bool is_white_space(char c);
bool is_word_separator(char c);
bool is_newline(char c);

char str_iter_get_char(str_iter_t* iter);
void str_iter_inc(str_iter_t* iter);
void str_iter_dec(str_iter_t* iter);
bool str_iter_match(str_iter_t* iter, str_t match_str);
bool str_iter_rmatch(str_iter_t* iter, str_t match_str);
int str_iter_match_int(str_iter_t* iter);
s64 str_iter_match_s64(str_iter_t* iter);
bool str_iter_is_digit(str_iter_t* iter);
bool str_iter_is_white_space(str_iter_t* iter);
bool str_iter_match_white_space(str_iter_t* iter);
bool str_iter_match_char(str_iter_t* iter, char match);
bool str_iter_match_newline(str_iter_t* iter);
str_t str_iter_match_up_to_char(str_iter_t* iter, char match);
str_t str_iter_match_word(str_iter_t* iter);
int_arr_t str_iter_match_int_list(str_iter_t* iter);

str_t get_input_file_name(char* day);
str_t get_part_one_test_file_name(char* day);
str_t get_part_two_test_file_name(char* day);

bool assert_equal(int result, int expected_result);

void check_part_one();
void check_part_two();
void compute_part_one();
void compute_part_two();

grid_t grid_from_str(str_t str);
void grid_add_row(grid_t* g, str_t row);
void grid_print(grid_t* g);
char grid_at(grid_t grid, int x, int y);
char* grid_pointer_at(grid_t grid, int x, int y);

s64 pow_s64(int base, int exponent);

#endif
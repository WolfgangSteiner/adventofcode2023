#include "aoc.h"
#include "test.h"

void test_str_reduce_char_spans() {
    str_t str = str_ref("...#....#..");
    str_t res = str_reduce_char_spans(str, '.');
    ASSERT_EQUAL_STR(res, str_ref(".#.#."));
}

void test_str_strip_char() {
    str_t str = str_ref("...#...#...");
    str_t res = str_strip_char(str, '.');
    str_t exp = str_ref("#...#");
    ASSERT_EQUAL_STR(res, exp)
}


int main(int, char**) {
    test_str_strip_char();
    test_str_reduce_char_spans();    
}
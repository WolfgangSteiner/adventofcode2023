#ifndef TEST_H
#define TEST_H

#include "aoc.h"

static inline void assert_equal_str(str_t a, str_t b, str_t file, int line) {
    if (!str_eq(a, b)) {
        str_t msg = str_format(str_ref("[ERROR] {str}:{int} Expected {str} but got {str}."), file, line, b, a);
        str_print(msg);
        str_free(msg);
        assert(false);
    }
}


static inline void assert_equal_int(int a, int b, str_t file, int line) {
    if (a != b) {
        str_t msg = str_format(str_ref("[ERROR] {str}:{int} Expected {int} but got {int}"), file, line, b, a);
        str_print(msg);
        str_free(msg);
        assert(false);
    }
}


static inline void assert_true(bool a, str_t file, int line) {
    if (!a) {
        str_t msg = str_format(str_ref("[ERROR] {str}:{int} Expected true but got false"), file, line);
        str_print(msg);
        str_free(msg);
        assert(false);
    }
}

static inline void assert_false(bool a, str_t file, int line) {
    if (a) {
        str_t msg = str_format(str_ref("[ERROR] {str}:{int} Expected false but got true"), file, line);
        str_print(msg);
        str_free(msg);
        assert(false);
    }
}


#define ASSERT_TRUE(A) \
    assert_true(A, str_ref(__FILE__), __LINE__); 

#define ASSERT_FALSE(A) \
    assert_false(A, str_ref(__FILE__), __LINE__); 

#define ASSERT_EQUAL_STR(A, B) \
    assert_equal_str(A, B, str_ref(__FILE__), __LINE__); 

#define ASSERT_EQUAL_INT(A, B) \
    assert_equal_int(A, B, str_ref(__FILE__), __LINE__); 

#endif
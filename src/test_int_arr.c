#include "aoc.h"
#include "test.h"

void test_int_arr_from_str() {
    str_t str = str_ref("1,2,3,4");
    int_arr_t arr = int_arr_from_str(str);
    ASSERT_EQUAL_INT(arr.size, 4);
}

int main(int, char**) {
    test_int_arr_from_str();
}
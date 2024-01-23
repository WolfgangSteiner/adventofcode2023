#include "test.h"
#include "strarr.h"

void test_str_split_char() {
    str_t str = str_ref("#.#.#");
    strarr_t res = str_split_char(str, '.');
    ASSERT_EQUAL_INT(res.size, 3);
}



int main(int, char**) {



}
#include "aoc.h"

char* day = "01";
s64 part_one_test_value = 142;
s64 part_two_test_value = 281;

char* digit_names[10] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int match_digit(str_iter_t* iter) {
    for (int i = 0; i <= 9; ++i) {
        if (str_iter_match(iter, str_ref(digit_names[i]))) return i;
    }
    return -1;
}

int rmatch_digit(str_iter_t* iter) {
    for (int i = 0; i <= 9; ++i) {
        if (str_iter_rmatch(iter, str_ref(digit_names[i]))) return i;
    }
    return -1;
}

int get_code(str_t str, bool include_words) {
    int digit_1 = 0, digit_2 = 0;
    str_iter_t iter = str_iter_begin(&str);
    while (!str_iter_is_end(&iter)) {
        char c = str_iter_get_char(&iter);
        if (is_digit(c)) {
            digit_1 = char_to_int(c);
            break;
        } else if (include_words && (digit_1 = match_digit(&iter)) > 0) {
            break; 
        }
        str_iter_inc(&iter);
    }    

    iter = str_iter_rbegin(&str);

    while (!str_iter_is_rend(&iter)) {
        char c = str_iter_get_char(&iter);
        if (is_digit(c)) {
            digit_2 = char_to_int(c);
            break;
        } else if (include_words && (digit_2 = rmatch_digit(&iter)) > 0) {
            break; 
        }
        str_iter_dec(&iter);
    }    

    return 10*digit_1 + digit_2;
}

int get_code_2(str_t str) {
    int digit_1, digit_2;
    (void) digit_1; 
    (void) digit_2;
    return 0; 
}

int compute_sum(str_t input, bool include_words) {
    str_iter_t iter = str_iter_begin(&input);
    int sum = 0;
    while (!str_iter_is_end(&iter)) {
        str_t line = get_line(&iter);
        int code = get_code(line, include_words);
        sum += code;
    }
    return sum;
}


s64 part_one(str_t input) {
   return compute_sum(input, false); 
}

s64 part_two(str_t input) {
    return compute_sum(input, true);
}

#include "aoc.h"
#include "strarr.h"
#include "test.h"

char* day = "12";
s64 part_one_test_value = 21;
s64 part_two_test_value = 525152 ;

#if 0
bool is_match_plausible(str_t substr, int length) {
    assert(substr.data[0] == '#' || substr.data[1] == '?');

    if (length == 0) return true;
    while (!str_iter_is_end(&find_iter)) {
        char c = str_iter_get_char(&find_iter);
        if (c == '#') {
            if (length == 0) return false;
            length--;
        } else if (c == '?') {
            if (length > 0) length--;
            else return true;
        } else if (c == '.') {
            return length == 0;
        }
    }  
    return length == 0;
}
#endif 


s64 count_leading_space(str_t str) {
    assert(str_at(str, 0) != '.');
    s64 count = 0;
    for (int i = 0; i < str.size; ++i) {
        char c = str_at(str, i);
        if (c == '.') return count;
        count++;
    }
    return count;
}

bool is_match_possible(str_t str, int match_length) {
    if (str.size == 0) return false;
    if (count_leading_space(str) < match_length) return false;
    else if (str.size == match_length) return true;
    else {
        char c = str_at(str, match_length);
        return c == '.' || c == '?';
    }   
}

bool str_has_enough_capacity(str_t str, int_arr_t groups) {
    int sum_groups = int_arr_sum(groups);
    int sum_space = groups.size - 1;
    s64 capacity = 0;
    s64 longest_span = 0;
    s64 span_length = 0;
    for (int i = 0; i < str.size; ++i) {
        char c = str_at(str, i);
        if (c == '?' || c == '#') {
            capacity++;
            span_length++;
            longest_span = MAX(longest_span, span_length);
        } else {
            span_length = 0;
        }
    }
    return capacity >= sum_groups 
        && str.size >= sum_groups + sum_space
        && longest_span >= int_arr_max(groups);
}


s64 count_single_match(str_t str, int match_length) {
    str_iter_t start_iter = str_find_char(&str, '#');
    str_iter_t end_iter = str_find_char_from_back(&str, '#');

    int group_size = end_iter.pos - start_iter.pos + 1;    

    for (int i = start_iter.pos; i < end_iter.pos; ++i) {
        if (str.data[i] == '.') return 0;
    }

    if (group_size == match_length) return 1;
    else if (group_size > match_length) return 0;

    int delta = match_length - group_size;
    int p1 = start_iter.pos;
    int p2 = end_iter.pos;

    for (int i = 0; i < delta; ++i) {
        if (p1 == 0) break;
        if (str.data[p1 - 1] == '?') p1--;
        else break;
    }

    for (int i = 0; i < delta; ++i) {
        if (p2 == str.size - 1) break;
        if (str.data[p2 + 1] == '?') p2++;
        else break;
    }

    int extended_group_size = p2 - p1;
    return extended_group_size - group_size + 1;
}


s64 count_possible_solutions(str_t str, int_arr_t groups) {
    str = str_lstrip_char(str, '.');
    
    
    if (groups.size == 1) {
        if (str_contains_char(str, '#')) {
            return count_single_match(str, groups.data[0]);
        }
    }
    else if (groups.size == 0) {
        if (str_contains_char(str, '#')) return 0;
        else return 1;
    } else if (str.size == 0) return groups.size == 0;
    else if (!str_has_enough_capacity(str, groups)) return 0;
    
    groups = int_arr_ref(groups);
    int next_match = int_arr_front(groups);

    if (str_starts_with_char(str, '#')) {
        if (is_match_possible(str, next_match)) {
            if (next_match == str.size) {
                return groups.size == 1;
            } else {
                str = str_substr(str, next_match + 1, -1);
                int_arr_pop_front(&groups);
                return count_possible_solutions(str, groups);
            }
        } else {
            return 0;
        }
    } else {
        if (is_match_possible(str, next_match)) {
            if (next_match == str.size) {
                return groups.size == 1;
            } else {
                s64 count = 0;
                str_t str1 = str_substr(str, 1, -1);
                count += count_possible_solutions(str1, groups);
                
                int_arr_pop_front(&groups);
                str_t str2 = str_substr(str, next_match + 1, -1);
                count += count_possible_solutions(str2, groups);
                return count;
            }
        } else {
            return count_possible_solutions(str_substr(str, 1, -1), groups);
        }
    }
    return 0;
}


s64 part_one(str_t input) {
    s64 sum = 0;
    str_iter_t iter = str_iter_begin(&input);
    while (!str_iter_is_end(&iter)) {
        str_t line = get_line(&iter);
        str_iter_t iter = str_iter_begin(&line);
        str_t symbol_str = str_iter_match_up_to_char(&iter, ' ');
        int_arr_t matches = str_iter_match_int_list(&iter);
        s64 count = count_possible_solutions(symbol_str, matches);
        sum += count;
        int_arr_free(matches);
    }
    return sum;
}


bool str_starts_with_complete_match(str_t str, int_arr_t matches) {
    for (int i = 0; i < int_arr_front(matches); ++i) {
        if (str_at(str, i) != '#') return false;
    }
    return true;
}

bool str_ends_with_complete_match(str_t str, int_arr_t matches) {
    for (int i = 0; i < int_arr_back(matches); ++i) {
        if (str_at(str, str.size - i - 1) != '#') return false;
    }
    return true;
}

str_t extend_str(str_t str) {
    str_t res = str_copy(str);
    for (int i = 0; i < 4; ++i) {
        str_append_char(&res, '?');
        str_append(&res, str);
    }
    return res;
}

void extend_matches(int_arr_t* arr) {
    int n = arr->size;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < n; ++j) {
            int_arr_push(arr, arr->data[j]);
        }
    }
}


s64 part_two(str_t input) {
    static bool skip = false;
    if (skip) return 0;
    s64 sum = 0;
    str_iter_t iter = str_iter_begin(&input);
    int line_number = 1;
    while (!str_iter_is_end(&iter)) {
        str_t line = get_line(&iter);
        str_iter_t iter = str_iter_begin(&line);
        str_t symbol_str = str_iter_match_up_to_char(&iter, ' ');
        int_arr_t matches = str_iter_match_int_list(&iter);
        
        str_print_format(str_ref("{int}: {str}"), line_number++, symbol_str);

        if (str_ends_with_char(symbol_str, '#')) {
            s64 count = count_possible_solutions(symbol_str, matches);
            sum += pow_s64(count, 5);
        } else {
            str_t extended_str = extend_str(symbol_str);
            extend_matches(&matches);
            sum += count_possible_solutions(extended_str, matches);
            str_free(extended_str);
        }

        #if 0
        if (str_starts_with_complete_match(symbol_str, matches)
            || str_ends_with_complete_match(symbol_str, matches)) {
            s64 count = count_possible_solutions(symbol_str, matches);
            sum += count * count * count * count * count;
        } else {
            str_t str1 = str_format(str_ref("{str}?"), symbol_str);
            str_t str2 = str_format(str_ref("?{str}?"), symbol_str);
            str_t str3 = str_format(str_ref("?{str}"), symbol_str);
            //s64 count0 = count_possible_solutions(symbol_str, matches);
            s64 count1 = count_possible_solutions(str1, matches);
            s64 count2 = count_possible_solutions(str2, matches);
            s64 count3 = count_possible_solutions(str3, matches);
            s64 count = count1 * count2 * count2 * count2 * count3;
            sum += count;
            str_free(str1);
            str_free(str2);
            str_free(str3);
        }
        //str_print_format(str_ref("{str} -> {s64}"), symbol_str, count);
        #endif
        int_arr_free(matches);
    }
    printf("\n");
    //skip=true;
    return sum;
}


void test_is_match_possible() {
    ASSERT_TRUE(is_match_possible(str_ref("###..."), 3));
    ASSERT_TRUE(is_match_possible(str_ref("##?..."), 3));
    ASSERT_TRUE(is_match_possible(str_ref("????..."), 3));
    ASSERT_FALSE(is_match_possible(str_ref("##...."), 3));
    ASSERT_TRUE(is_match_possible(str_ref("?##..."), 3));
    ASSERT_FALSE(is_match_possible(str_ref("?###..."), 3));
    ASSERT_TRUE(is_match_possible(str_ref("###"), 3));
}


void test_count_single_match() {
    ASSERT_EQUAL_INT(count_single_match(str_ref("...#???#..."), 5), 1);
    ASSERT_EQUAL_INT(count_single_match(str_ref("...#?.?#..."), 5), 0);
    ASSERT_EQUAL_INT(count_single_match(str_ref("...?##."), 3), 1);
}


void test_part_one() {
    ASSERT_EQUAL_INT(count_possible_solutions(
        str_ref("???.###"), int_arr_from_str(str_ref("1,1,3"))), 1);
    ASSERT_EQUAL_INT(count_possible_solutions(
        str_ref(".??..??...?##."), int_arr_from_str(str_ref("1,1,3"))), 4);
    ASSERT_EQUAL_INT(count_possible_solutions(
        str_ref("?#?#?#?#?#?#?#?"), int_arr_from_str(str_ref("1,3,1,6"))), 1);
}


void run_tests() {
    test_is_match_possible();
    test_count_single_match();
    test_part_one();
}

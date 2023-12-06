#include "aoc.h"

char* day = "06";
s64 part_one_test_value = 288;
s64 part_two_test_value = 71503;

typedef struct {
    int_arr_t times;
    int_arr_t distances;
} pd_t;

pd_t parse_input(str_t input) {
    pd_t pd = {0};
    str_iter_t iter = str_iter_begin(&input);
    str_iter_match(&iter, str_ref("Time:")); 
    pd.times = str_iter_match_int_list(&iter);
    str_iter_match_newline(&iter);
    str_iter_match(&iter, str_ref("Distance:"));
    pd.distances = str_iter_match_int_list(&iter);

    return pd;
}

s64 get_int_ignoring_white_space(str_iter_t* iter) {
    s64 val = 0;
    while (!str_iter_is_end(iter)) {
        char c = str_iter_get_char(iter);
        if (is_digit(c)) {
            val = val * 10 + char_to_int(c);
            str_iter_inc(iter);
        } else if (is_white_space(c)) {
            str_iter_inc(iter); 
        } else {
            break;
        }
    }
    return val;
}

typedef struct {
    s64 time;
    s64 distance;
} pd2_t;

pd2_t parse_input_part_two(str_t input) {
    pd2_t pd = {0};
    str_iter_t iter = str_iter_begin(&input);
    str_iter_match(&iter, str_ref("Time:"));
    s64 time = get_int_ignoring_white_space(&iter);
    str_iter_match_newline(&iter);
    str_iter_match(&iter, str_ref("Distance:"));
    s64 distance = get_int_ignoring_white_space(&iter);
    return (pd2_t) { .time=time, .distance=distance };
}

s64 distance_func(s64 t, s64 t_total) {
    return t * (t_total - t);
}

bool is_time_winning(s64 t, s64 t_total, s64 distance) {
    return distance_func(t, t_total) > distance;
}

s64 find_min_max_winning_time(s64 t_total, s64 distance, s64 t1, s64 t2) {
    s64 t_opt = t_total / 2;
    s64 t = 0;

    while (t2 - t1 > 1) {
        t = t1 + (t2 - t1) / 2;
        if (is_time_winning(t, t_total, distance)) {
            if (t < t_opt) t2 = t; else t1 = t;
        } else {
            if (t < t_opt) t1 = t; else t2 = t;
        }
        assert(t>=t1 && t<=t2);
    }

    bool is_t1_winning = is_time_winning(t1, t_total, distance);
    bool is_t2_winning = is_time_winning(t2, t_total, distance);
    assert(is_t1_winning != is_t2_winning);

    return is_t1_winning ? t1 : t2;
}

s64 num_possible_solutions(s64 time, s64 distance) {
    s64 t_opt = time / 2;
    s64 t_min = find_min_max_winning_time(time, distance, 1, t_opt);
    s64 t_max = find_min_max_winning_time(time, distance, t_opt, time);
    return t_max - t_min + 1;
}


s64 part_one(str_t input) {
    pd_t pd = parse_input(input);
    s64 res = 1;
    for (int i = 0; i < pd.times.size; ++i) {
        s64 t = int_arr_at(pd.times, i);
        s64 d = int_arr_at(pd.distances, i);
        s64 c = num_possible_solutions(t, d);
        res *= c;
    }

    return res;
}

s64 part_two(str_t input) {
    pd2_t pd = parse_input_part_two(input);
    return num_possible_solutions(pd.time, pd.distance);
}

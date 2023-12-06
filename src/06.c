#include "aoc.h"

char* day = "06";
s64 part_one_test_value = 288;
s64 part_two_test_value = 0;

typedef struct {
    int_arr_t times;
    int_arr_t distances;
} pd_t;

pd_t parse_input(str_t input) {
    pd_t pd;
    str_iter_t iter = str_iter_begin(&input);
    str_iter_match(&iter, str_ref("Time:")); 
    pd.times = str_iter_match_int_list(&iter);
    str_iter_match_newline(&iter);
    str_iter_match(&iter, str_ref("Distance:"));
    pd.distances = str_iter_match_int_list(&iter);

    return pd;
}

s64 distance_func(s64 t, s64 t_total) {
    return t * (t_total - t);
}

bool is_time_winning(s64 t, s64 t_total, s64 distance) {
    return distance_func(t, t_total) > distance;
}

s64 find_min_winning_time(s64 time, s64 distance) {
    s64 t_opt = time / 2;
    s64 t1 = 1;
    s64 t2 = t_opt;

    s64 t = 0;

    while (t1 != t2 && (t2 - t1) > 1) {
        t = t1 + (t2 - t1) / 2;
        printf("(t=%ld, t1=%ld, t2=%ld)\n", t, t1, t2);
        if (is_time_winning(t, time, distance)) {
            t2 = t;
        } else {
            t1 = t;
        }
    }

    assert(is_time_winning(t, time, distance));
    assert(!is_time_winning(t-1, time, distance));

    return t;
}

s64 num_possible_solutions(s64 time, s64 distance) {
    s64 t_opt = time / 2;
    s64 count = 1;
    s64 t = t_opt - 1;
    s64 t_min = find_min_winning_time(time, distance);
    count += (t_opt - t_min);
    t = t_opt + 1;
    while (distance_func(t, time) > distance) {
        count++;
        t++;
    }    

    return count;
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
    (void) input;
    return 0;
}

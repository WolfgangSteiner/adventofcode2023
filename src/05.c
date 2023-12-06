#include "aoc.h"

char* day = "05";
s64 part_one_test_value = 35;
s64 part_two_test_value = 46;

typedef struct {
    s64 start, end;
} range_t;

typedef struct {
    s64 src, dst, length;
} mapping_t;


typedef struct {
    str_t name;
    mapping_t* mappings;
    size_t size;
    size_t capacity;
} map_t;

s64 mapping_map_value(mapping_t m, s64 v) {
    return v - m.src + m.dst;
}

range_t mapping_map_range(mapping_t m, range_t r) {
    return (range_t) { 
        .start=mapping_map_value(m, r.start),
        .end=mapping_map_value(m, r.end)
    };
}

map_t map_create(str_t name) {
    return (map_t) {
        .name=str_copy(name),
        .mappings=malloc(64*sizeof(mapping_t)),
        .size=0,
        .capacity=1024
    };
}

void map_push_mapping(map_t* map, mapping_t m) {
    if (map->size == map->capacity) {
        map->capacity *= 2;
        map->mappings = realloc(map->mappings, map->capacity);
    }
    map->mappings[map->size++] = m; 
}

typedef struct {
    int_arr_t seeds;
    map_t* maps;
    int map_count;
} puzzle_data_t;


puzzle_data_t parse_input(str_t input) {
    puzzle_data_t pd = {0};
    pd.maps = calloc(128, sizeof(map_t));
    pd.map_count = 0;
    map_t* cur_map = 0;

    str_iter_t line_iter = str_iter_begin(&input);

    while(!str_iter_is_end(&line_iter)) {
        str_t line = get_line(&line_iter);
        str_iter_t iter = str_iter_begin(&line);
        if (str_iter_match(&iter, str_ref("seeds:"))) {
            pd.seeds = str_iter_match_int_list(&iter);
        } else if (str_contains(line, str_ref("map:"))) {
            str_t map_name = str_iter_match_word(&iter);
            cur_map = pd.maps + pd.map_count;
            *cur_map = map_create(map_name);
            pd.map_count++;
        } else if (str_iter_is_digit(&iter)) {
            mapping_t m;
            m.dst = str_iter_match_s64(&iter);
            m.src = str_iter_match_s64(&iter);
            m.length = str_iter_match_s64(&iter);
            map_push_mapping(cur_map, m);
        }
    }

    return pd;
}

s64 apply_map(map_t map, s64 val) {
    for (int i = 0; i < map.size; ++i) {
        mapping_t m = map.mappings[i];
        if (val >= m.src && val < m.src + m.length) {
            s64 offset = val - m.src;
            return m.dst + offset;
        }
    }
    return val;
}

s64 apply_maps(puzzle_data_t pd, s64 val) {
    for (int i = 0; i < pd.map_count; ++i) {
        val = apply_map(pd.maps[i], val);
    }

    return val;
}

typedef struct {
    range_t* stack;
    size_t capacity;
    size_t size;
} range_stack_t;

range_stack_t range_stack_create() {
    range_stack_t s;
    s.stack = calloc(512, sizeof(range_t));
    s.capacity = 512;
    s.size = 0;
    return s;
}

void range_stack_push(range_stack_t* s, range_t r) {
    if (s->size == s->capacity) {
        s->capacity *= 2;
        s->stack = realloc(s->stack, s->capacity);
    }
    s->stack[s->size++] = r;
}

range_t range_stack_pop(range_stack_t* s) {
    s->size--;
    return s->stack[s->size];
}

bool range_stack_empty(range_stack_t* s) {
    return s->size == 0;
}

bool ranges_intersect(range_t a, range_t b) {
    return a.start <= b.end && a.end >= b.start;
}

s64 compute_min_loc_for_map_level(puzzle_data_t pd, int map_level, range_t a) {
    s64 min_loc = S64_MAX;
    int max_map_level = pd.map_count - 1;
    range_stack_t incoming_stack = range_stack_create();
    range_stack_push(&incoming_stack, a);
    map_t* map = &pd.maps[map_level];
    
    while (!range_stack_empty(&incoming_stack)) {
        range_t range = range_stack_pop(&incoming_stack);
        range_t mapped_range = range; 
        for (int i = 0; i < map->size; ++i) {
            mapping_t m = map->mappings[i];
            range_t mapping_range = { .start=m.src, .end=m.src+m.length-1 };
            if (ranges_intersect(mapping_range, range)) {
                if (range.start < mapping_range.start) {
                    range_t left_range = { .start=range.start, .end=mapping_range.start-1 };
                    range.start = mapping_range.start;
                    range_stack_push(&incoming_stack, left_range);
                }
                if (range.end > mapping_range.end) {
                    range_t right_range = { .start=mapping_range.end + 1, .end=range.end };
                    range.end = mapping_range.end;
                    range_stack_push(&incoming_stack, right_range);
                }
                mapped_range = mapping_map_range(m, range);

                break;
            }
        }
    
        if (map_level == max_map_level) {
            min_loc = min_s64(mapped_range.start, min_loc);
        } else {
            min_loc = min_s64(
                min_loc,
                compute_min_loc_for_map_level(pd, map_level + 1, mapped_range));
        }
    }    

    return min_loc;
}

s64 compute_min_loc(puzzle_data_t pd) {
    s64 min_loc = S64_MAX;
    
    for (int i = 0; i < pd.seeds.size; ++i) {
        s64 seed = int_arr_at(pd.seeds, i);
        s64 loc = apply_maps(pd, seed);
        min_loc = min_s64(loc, min_loc);
    }  

    return min_loc;
}

s64 compute_min_loc_ranges(puzzle_data_t pd) {
    s64 min_loc = S64_MAX;
    for (int i = 0; i < pd.seeds.size; i+=2) {
        s64 start = int_arr_at(pd.seeds, i);
        s64 length = int_arr_at(pd.seeds, i + 1);
        range_t r = { start, start + length - 1 };
        min_loc = min_s64(min_loc, compute_min_loc_for_map_level(pd,  0, r));
    }  

    return min_loc;
}

s64 part_one(str_t input) {
    puzzle_data_t pd = parse_input(input);
    return compute_min_loc(pd);
}

s64 part_two(str_t input) {
    puzzle_data_t pd = parse_input(input);
    return compute_min_loc_ranges(pd);
}

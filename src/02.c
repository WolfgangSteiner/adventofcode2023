#include "aoc.h"

char* day = "02";
s64 part_one_test_value = 8;
s64 part_two_test_value = 2286;

typedef struct {
    int red, green, blue;
} sample_t;

typedef struct {
    sample_t* samples;
    size_t capacity;
    size_t size;
    int id;
} game_t;

game_t new_game() {
    game_t g;
    g.samples = calloc(1024, sizeof(sample_t));
    g.capacity = 1024;
    g.size = 0;
    g.id = 0;
    return g;
}

sample_t* add_sample(game_t* game) {
    if (game->size == game->capacity) {
        game->capacity *= 2;
        game->samples = realloc(game->samples, game->capacity * sizeof(sample_t));
    }
    game->samples[game->size++] = (sample_t){0};
    return &game->samples[game->size - 1];
}

game_t parse_line(str_t str) {
    game_t g = new_game();
    str_iter_t iter = str_iter_begin(&str);
    bool match_game = str_iter_match(&iter, str_ref("Game "));
    assert(match_game);
    g.id = str_iter_match_int(&iter);
    str_iter_match(&iter, str_ref(": "));
    sample_t* s = add_sample(&g);
    while (!str_iter_is_end(&iter)) {
        char c = str_iter_get_char(&iter);
        if (c == ';') {
            s = add_sample(&g);
            str_iter_inc(&iter);
            str_iter_match_white_space(&iter);
        } else if (c == ',') {
            str_iter_inc(&iter);
            str_iter_match_white_space(&iter);
        } else {
            int count = str_iter_match_int(&iter);
            str_iter_match_white_space(&iter);
            if (str_iter_match(&iter, str_ref("red"))) {
                s->red = count;
            } else if (str_iter_match(&iter, str_ref("green"))) {
                s->green = count;
            } else if (str_iter_match(&iter, str_ref("blue"))) {
                s->blue = count;
            } else {
                assert(false);
            }
        }
    }
    return g;
}

sample_t get_max(game_t* g) {
    sample_t max_samples = {0};
    for (int i = 0; i < g->size; ++i) {
        sample_t s = g->samples[i];
        max_samples.red = MAX(max_samples.red, s.red);
        max_samples.green = MAX(max_samples.green, s.green);
        max_samples.blue = MAX(max_samples.blue, s.blue);
    }
    return max_samples;
}

int is_game_possible(game_t* g) {
    sample_t max_samples = get_max(g);
    sample_t num_balls = {12, 13, 14};
    bool is_possible = max_samples.red <= num_balls.red &&
        max_samples.green <= num_balls.green &&
        max_samples.blue <= num_balls.blue;
    if (is_possible) return g->id;
    else return 0;
}  

int compute_min_power(game_t* g) {
    sample_t max_samples = get_max(g);
    return max_samples.red * max_samples.green * max_samples.blue;
}

typedef int (game_eval_func)(game_t*);

int compute_score(str_t input, game_eval_func f) {
    str_iter_t iter = str_iter_begin(&input);
    int sum = 0;
    while (!str_iter_is_end(&iter)) {
        str_t line = get_line(&iter);
        game_t game = parse_line(line);
        sum += f(&game);
    }
    return sum;
}

s64 part_one(str_t input) {
    int res = compute_score(input, is_game_possible);
    return res;
}

s64 part_two(str_t input) {
    int res = compute_score(input, compute_min_power);
    return res;
}

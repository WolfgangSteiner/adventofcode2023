#include "aoc.h"

char* day = "03";
s64 part_one_test_value = 4361;
s64 part_two_test_value = 467835;

typedef struct {
    int x, y, length;
    int gear_x, gear_y;
    int value;
} number_t;

typedef struct {
    number_t* data;
    int capacity;
    int size;
} number_list_t;

number_list_t number_list_make() {
    number_list_t list;
    list.capacity = 1024;
    list.size = 0;
    list.data = calloc(list.capacity, sizeof(number_t));
    return list;
}

void number_list_free(number_list_t number_list) {
    free(number_list.data);
}

void number_list_push(number_list_t* list, number_t number) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(number_t));
    }
    list->data[list->size++] = number;
}

number_t number_list_at(number_list_t number_list, int idx) {
    return number_list.data[idx];
}

void number_list_clear(number_list_t* number_list) {
    number_list->size = 0;
}

bool is_number_adjacent_to_symbol(number_t number, grid_t grid) {
    int y1 = MAX(number.y - 1, 0);
    int y2 = MIN(number.y + 1, grid.height - 1);
    int x1 = MAX(number.x - 1, 0);
    int x2 = MIN(number.x + number.length, grid.width - 1);

    for (int row = y1; row <= y2; ++row) {
        for (int col = x1; col <= x2; ++col) {
            char c = grid_at(grid, col, row);
            if (!is_digit(c) && !(c == '.')) {
                return true;
            }
        }
    }
    return false;
}

number_t get_number(grid_t grid, int y, int x_start, int length) {
    number_t res = {0};
    str_t str = { .data=grid_pointer_at(grid, x_start, y), .size=length, .is_valid=true};
    res.value = str_to_int(str); 
    res.x = x_start;
    res.y = y;
    res.length = length;
    res.gear_x = -1;
    res.gear_y = -1;
    return res;
}

number_list_t collect_numbers(grid_t grid) {
    number_list_t number_list = number_list_make();
    for (int y = 0; y < grid.height; ++y) {
        int span_start = -1, span_length = -1;
        for (int x = 0; x < grid.width; ++x) {
            char c = grid_at(grid, x, y);
            if (is_digit(c)) {
                if (span_start < 0) {
                    span_start = x;
                    span_length = 0;
                }
                span_length++;
            } 
            
            if ((x == grid.width - 1) || !is_digit(grid_at(grid, x+1, y))) {
                if (span_start >= 0) {
                    number_t number = get_number(grid, y, span_start, span_length);
                    number_list_push(&number_list, number);
                    span_start = -1;
                    span_length = -1;
                }   
            }
        }
    }
    return number_list;
}

int count_numbers(grid_t grid, number_list_t number_list) {
    int sum = 0;
    for (int i = 0; i < number_list.size; ++i) {
        number_t number = number_list_at(number_list, i);
        if (is_number_adjacent_to_symbol(number, grid)) sum += number.value;
    }
    return sum;
}

int part_one(str_t input) {
    grid_t grid = grid_from_str(input);
    number_list_t number_list = collect_numbers(grid);
    int res = count_numbers(grid, number_list);
    return res;
}

bool check_number_is_part_of_gear(grid_t grid, number_t* number) {
    int y1 = MAX(number->y - 1, 0);
    int y2 = MIN(number->y + 1, grid.height - 1);
    int x1 = MAX(number->x - 1, 0);
    int x2 = MIN(number->x + number->length, grid.width - 1);

    for (int y = y1; y <= y2; ++y) {
        for (int x = x1; x <= x2; ++x) {
            char c = grid_at(grid, x, y);
            if (c == '*') {
                number->gear_x = x;
                number->gear_y = y;
                return true;
            }
        }
    }
    return false;
}

number_list_t get_numbers_adjecent_to_gears(number_list_t number_list, grid_t grid) {
    number_list_t res = number_list_make();
    for (int i = 0; i < number_list.size; ++i) {
        number_t number = number_list_at(number_list, i);
        bool is_part_of_gear = check_number_is_part_of_gear(grid, &number);
        if (is_part_of_gear) {
            number_list_push(&res, number);
        }
    }
    return res;
}

s64 gear_ratio(number_list_t number_list, number_list_t* adjacency_list, int x, int y) {
    int res = 0;
    number_list_clear(adjacency_list);
    for (int idx = 0; idx < number_list.size; ++idx) {
        number_t number = number_list_at(number_list, idx);
        if (number.gear_x == x && number.gear_y == y) {
            number_list_push(adjacency_list, number);
            if (adjacency_list->size == 2) {
                int v1 = adjacency_list->data[0].value;
                int v2 = adjacency_list->data[1].value;
                res = v1 * v2;
                return res;
            }
        }
    }

    return res;
}

s64 part_two(str_t input) {
    grid_t grid = grid_from_str(input);
    number_list_t number_list = collect_numbers(grid);
    number_list_t numbers_adjacent_to_gears = get_numbers_adjecent_to_gears(number_list, grid);    
    number_list_t adjacency_list = number_list_make();

    int sum = 0;
    for (int y = 0; y < grid.height; ++y) {
        for (int x = 0; x < grid.width; ++x) {
            char c = grid_at(grid, x, y);
            if (c == '*') {
                sum += gear_ratio(numbers_adjacent_to_gears, &adjacency_list, x, y);
            }
        }
    }

    return sum;
}

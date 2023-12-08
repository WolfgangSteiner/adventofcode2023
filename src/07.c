#include "aoc.h"

char* day = "07";
s64 part_one_test_value = 6440;
s64 part_two_test_value = 5905;
    
static str_t card_values_str = { .data="0123456789TJQKA", .size=15, .owns_data=false, .is_valid=true};
static int cards_per_hand = 5;

typedef struct {
    int cards[5];
    int value;
    int bid;
} hand_t;

void log(char* msg) { printf("%s\n", msg);}

hand_t hand_make(str_t line, bool has_jokers) {
    hand_t hand = {0};
    str_t cards_str = str_substr(line, 0, 5);
    str_t bid_str = str_lstrip(str_substr(line, 5, -1));
    hand.bid = str_to_int(bid_str);
    for (int i = 0; i < 5; ++i) {
        char c = str_at(cards_str, i);
        str_iter_t find_iter = str_find_char(card_values_str, c);
        assert(!str_iter_is_end(&find_iter));
        int value = find_iter.pos;
        if (has_jokers && value == 11) value = 0;
        hand.cards[i] = value;
    }

    return hand;
}

bool int_arr_any(int_arr_t arr, s64 value) {
    for (int i = 0; i < arr.size; ++i) {
        if (arr.data[i] == value) return true;
    }
    return false;
}

bool int_arr_one(int_arr_t arr, s64 value) {
    bool value_found = false;
    for (int i = 0; i < arr.size; ++i) {
        if (arr.data[i] == value) {
            if (!value_found) value_found = true;
            else return false;
        }       
    }
    return value_found;
}

int int_arr_count(int_arr_t arr, s64 value) {
    int count = 0;
    for (int i = 0; i < arr.size; ++i) {
        if (arr.data[i] == value) {
            count++;
        }       
    }
    return count;
}

s64 int_arr_max(int_arr_t arr) {
    s64 max_value = -S64_MAX;
    for (int i = 0; i < arr.size; ++i) {
        max_value = max_s64(max_value, arr.data[i]);
    }
    return max_value;
}

static size_t num_card_values = 15;

bool is_single_pair(int_arr_t arr, int num_jokers) {
    if (num_jokers == 0) {
        return int_arr_one(arr, 2) && int_arr_max(arr) == 2;
    } else {
        return num_jokers == 1 && int_arr_max(arr) == 1;
    }
}

bool is_two_pair(int_arr_t arr, int num_jokers) {
    return num_jokers == 0 && int_arr_count(arr, 2) == 2;
}

bool is_three_of_a_kind(int_arr_t arr, int num_jokers) {
    if (num_jokers == 0) {
        return int_arr_any(arr, 3) && !int_arr_any(arr, 2);
    } else {
        return (num_jokers == 1 && is_single_pair(arr, 0))
          || (num_jokers == 2 && int_arr_max(arr) == 1);
    }
}

bool is_full_house(int_arr_t arr, int num_jokers) {
    return  ((num_jokers == 0) && int_arr_any(arr, 3) && int_arr_any(arr, 2))
      || (num_jokers == 1 && is_two_pair(arr, 0));
}

bool is_four_of_a_kind(int_arr_t arr, int num_jokers) {
    if (num_jokers == 0) {
        return int_arr_any(arr, 4);
    } else {
        return (num_jokers == 3 && int_arr_max(arr) == 1)
        || (num_jokers == 2 && is_single_pair(arr, 0))
        || (num_jokers == 1 && is_three_of_a_kind(arr, 0));
    }
}

bool is_five_of_a_kind(int_arr_t arr, int num_jokers) {
    if (num_jokers == 0) {
        return int_arr_any(arr, 5);
    } else {
        return num_jokers >= 4
            || (num_jokers == 3 && is_single_pair(arr, 0))
            || (num_jokers == 2 && is_three_of_a_kind(arr, 0))
            || (num_jokers == 1 && is_four_of_a_kind(arr, 0));
    }
}

int hand_value(hand_t hand, bool has_jokers) {
    s64 values[15] = {0};
    int_arr_t value_arr = {.data=values, .size=15};
    for (int i = 0; i < 5; ++i) {
        values[hand.cards[i]]++;
    }

    int num_jokers = 0;

    if (has_jokers) {
        assert(values[11] == 0);
        num_jokers = values[0];
        values[0] = 0;
    }

    if (is_five_of_a_kind(value_arr, num_jokers)) {
        return 6;
    } else if (is_four_of_a_kind(value_arr, num_jokers)) {
        return 5;
    } else if (is_full_house(value_arr, num_jokers)) {
        return 4;
    } else if (is_three_of_a_kind(value_arr, num_jokers)) {
        return 3;
    } else if (is_two_pair(value_arr, num_jokers)) {
        return 2;
    } else if (is_single_pair(value_arr, num_jokers)) {
        return 1;
    }

    return 0;
}

typedef struct {
    hand_t* arr;
    size_t capacity; 
    size_t size;
} hand_arr_t;


void hand_arr_push(hand_arr_t* arr, hand_t hand) {
    if (arr->arr == 0) {
        arr->capacity = 1024;
        arr->arr = calloc(arr->capacity, sizeof(hand_t));
        arr->size = 0;
    } else if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->arr = realloc(arr->arr, arr->capacity * sizeof(hand_t));
    }
    arr->arr[arr->size++] = hand;
}


bool hand_cmp(hand_t* a, hand_t* b) {
    if (a->value < b->value) {
        return true;
    } else if (a->value > b->value) {
        return false;
    } else {
        int i = 0;
        while (a->cards[i] == b->cards[i] && i < cards_per_hand - 1) ++i;
        return a->cards[i] < b->cards[i];
    }
}

void hand_swp(hand_t* a, hand_t* b) {
    hand_t t = *a; *a = *b; *b = t;
}

void hand_arr_sort_impl(hand_arr_t arr, size_t start, size_t end) {
    if (start == end) return;
    else if (end - start == 1) {
        hand_t* a = arr.arr + start;
        hand_t* b = arr.arr + end;
        if (!hand_cmp(a,b)) hand_swp(a, b);
    } else {
        size_t split = (start + end) / 2;
        hand_arr_sort_impl(arr, start, split);
        hand_arr_sort_impl(arr, split + 1, end);
        hand_t* left = arr.arr + start;
        hand_t* right = arr.arr + split + 1;
        hand_t* end_ptr = arr.arr + end;
        
        while(left < right && right <= end_ptr) {
            if (!hand_cmp(left, right)) {
                hand_t t = *right;
                hand_t* mv_ptr = right;
                while (mv_ptr > left) {
                    *mv_ptr = *(mv_ptr - 1);
                    mv_ptr--;
                }
                *left = t;
                right++;
            }
            left++;
        }

    } 
}

void hand_arr_sort(hand_arr_t arr) {
    hand_arr_sort_impl(arr, 0, arr.size - 1);
}

hand_arr_t parse_input(str_t input, bool has_jokers) {
    hand_arr_t arr = {0};
    str_iter_t line_iter = str_iter_begin(&input);
    while(!str_iter_is_end(&line_iter)) {
        str_t line = get_line(&line_iter);
        hand_t hand = hand_make(line, has_jokers);
        hand_arr_push(&arr, hand);
    }
    return arr;
}

void assign_values(hand_arr_t arr, bool has_jokers) {
    for (int i = 0; i < arr.size; ++i) {
        hand_t* hand = arr.arr + i;
        hand->value = hand_value(*hand, has_jokers);
    }
}


void hand_arr_print(hand_arr_t arr) {
    for (int i = 0; i < arr.size; ++i) {
        str_t msg = str_format(str_ref("{hand}"), arr.arr[i]);
        str_print(msg);
        str_free(msg);
    }
}

str_t format_hand(va_list* args, str_t) {
    hand_t hand = va_arg(*args, hand_t);

    str_t cards_str = str_new("");
    for (int i = 0; i < 5; ++i) {
        int value = hand.cards[i];
        str_append_char(&cards_str, card_values_str.data[value]);
    }

    str_t res = str_format(str_ref("{str}: bid={int} value={int}"), cards_str, hand.bid, hand.value); 
    str_free(cards_str);
    return res;
}

s64 score(hand_arr_t arr) {
    s64 s = 0;
    for (int i = 0; i < arr.size; ++i) {
        int rank = i + 1;
        s += rank * arr.arr[i].bid;
    }
    return s;
}

s64 part_one(str_t input) {
    static bool init = false;
    if (!init) {
        str_format_register_pattern(str_ref("hand"), format_hand);
        init = true;
    }

    hand_arr_t arr = parse_input(input, false);
    assign_values(arr, false);
    hand_arr_sort(arr);
    return score(arr);
}

s64 part_two(str_t input) {
    hand_arr_t arr = parse_input(input, true);
    assign_values(arr, true);
    hand_arr_sort(arr);
    return score(arr);
}

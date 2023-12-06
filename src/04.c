#include "aoc.h"

char* day = "04";
s64 part_one_test_value = 13;
s64 part_two_test_value = 30;

typedef struct {
    int count;
    int num_hits;
} card_t;

typedef struct {
    int size;
    card_t* cards;
} card_list_t;


void test_insertion_sort() {
    int_arr_t l = int_arr_create();
    int input[5] = { 6,3,7,1,8 };
    for (int i = 0; i < 5; ++i) {
        int_arr_insert_sorted(&l, input[i]);
        int_arr_print(&l);
    }
    assert(l.data[0] == 1);
}


s64 score_card(card_t card) {
    return card.num_hits ? pow_s64(2, card.num_hits - 1) : 0;
}


s64 compute_total_score(card_list_t card_list) {
    s64 sum = 0;
    for (int i = 0; i < card_list.size; ++i) {
        card_t card = card_list.cards[i];
        sum += score_card(card) * card.count;
    } 
    return sum;
}

s64 count_hits(int_arr_t* list_a, int_arr_t* list_b) {
    int count = 0;
    for (int i = 0; i < list_b->size; ++i) {
        int val = list_b->data[i];
        if (int_arr_sorted_contains(list_a, val)) ++count; 
    }
    return count;
}

card_list_t read_cards(str_t input) {
    card_list_t card_list = { .size=0, .cards=malloc(4096*sizeof(card_t))};
    str_iter_t line_iter = str_iter_begin(&input);
    int_arr_t list_a = int_arr_create();
    int_arr_t list_b = int_arr_create();

    while(!str_iter_is_end(&line_iter)) {
        int_arr_clear(&list_a);
        int_arr_clear(&list_b);
        int_arr_t* cur_list = &list_a;
        str_t line = get_line(&line_iter);
        str_iter_t iter = str_iter_begin(&line);
        str_iter_match(&iter, str_ref("Card"));
        str_iter_match_white_space(&iter);
        int card_number = str_iter_match_int(&iter);
        //printf("card number: %d\n", card_number);
        (void)card_number;
        str_iter_match_char(&iter, ':');
        while(!str_iter_is_end(&iter)) {
            str_iter_match_white_space(&iter);
            char c = str_iter_get_char(&iter);
            if (c == '|') {
                cur_list = &list_b;
                str_iter_inc(&iter);
            } else {
                int v = str_iter_match_int(&iter);
                int_arr_insert_sorted(cur_list, v);
            }
        }
        card_t card = { .count=1, .num_hits=count_hits(&list_a, &list_b) };
        card_list.cards[card_list.size++] = card;
    }

    return card_list;
}

void add_extra_cards(card_list_t card_list) {
    int num_cards = card_list.size;
    for (int i = 0; i < num_cards; ++i) {
        card_t card = card_list.cards[i];
        int count = card.num_hits;
        for (int j = 1; j <= count; j++) {
            if (i + j >= num_cards) break;
            card_list.cards[i+j].count += card.count;
        }
    }
}

s64 count_total_cards(card_list_t card_list) {
    s64 sum = 0;
    for (int i = 0; i < card_list.size; ++i) {
        card_t card = card_list.cards[i];
        sum += card.count;
    }
    return sum;
}


s64 part_one(str_t input) {
    card_list_t card_list = read_cards(input);
    return compute_total_score(card_list); 
}

s64 part_two(str_t input) {
    card_list_t card_list = read_cards(input);
    add_extra_cards(card_list);
    return count_total_cards(card_list);
}

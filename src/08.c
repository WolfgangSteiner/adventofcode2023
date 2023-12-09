#include "aoc.h"

char* day = "08";
s64 part_one_test_value = 2;
s64 part_two_test_value = 6;

static u32 max_num_ids = 26*26*26;
typedef u16 id_t;

typedef struct {
   id_t left, right; 
} node_t;


typedef struct {
    str_t instructions;
    node_t* nodes;
} pd_t;


pd_t pd_make() {
    return (pd_t) {
    .nodes = calloc(max_num_ids, sizeof(node_t))
    };
}

u16 str_to_id(str_t str) {
    u16 first = str.data[0] - 'A';
    u16 second = str.data[1] - 'A';
    u16 third = str.data[2] - 'A';
    return first * 26 * 26 + second * 26 + third;
}



pd_t parse_input(str_t input) {
    pd_t pd = pd_make();

    str_iter_t iter = str_iter_begin(&input);
    pd.instructions = get_line(&iter);
    // skip empty line
    get_line(&iter);
    while (!str_iter_is_end(&iter)) {
        str_t line = get_line(&iter);
        id_t id = str_to_id(str_substr(line, 0, 3));
        id_t left = str_to_id(str_substr(line, 7, 3));
        id_t right = str_to_id(str_substr(line, 12, 3));
        pd.nodes[id] = (node_t) {.left=left, .right=right};
    }    
    return pd;
}

int count_steps(pd_t pd) {
    id_t start_id = 0;
    id_t end_id = max_num_ids - 1;
    assert(str_to_id(str_ref("ZZZ")) == end_id);
    id_t cur_id = start_id;
    int count = 0;
    int ip = 0;
    int instr_count = pd.instructions.size;
    while (cur_id != end_id) {
        char instr = pd.instructions.data[ip];
        ip = (ip + 1) % instr_count;
        node_t n = pd.nodes[cur_id];
        if (instr == 'L') cur_id = n.left;
        else cur_id = n.right;
        count++;
    }
    return count;
}

typedef struct {
    id_t* data;
    size_t capacity;
    size_t size;
} idarr_t;

void idarr_push(idarr_t* arr, id_t id) {
    if (arr->data == 0) {
        arr->capacity = 1024;
        arr->size = 0;
        arr->data = calloc(arr->capacity, sizeof(id_t));
    } else if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(id_t));
    }
    arr->data[arr->size++] = id;
}

idarr_t collect_start_ids(pd_t pd) {
    idarr_t arr = {0};
    for (id_t id = 0; id < max_num_ids; id+=26) {
        node_t n = pd.nodes[id];
        if (n.left != n.right) {
            idarr_push(&arr, id);
        } else {
            assert(n.left == 0 && n.right == 0);
        }
    }
    return arr;
}

bool all_ids_end(idarr_t arr) {
    for (int i = 0; i < arr.size; ++i) {
        id_t id = arr.data[i];
        bool is_end = (id % 26 == 25);
        if (!is_end) return false;
    }
    return true;
}

void advance_ids_left(idarr_t arr, node_t* nodes) {
    for (int i = 0; i < arr.size; ++i) {
        id_t id = arr.data[i];
        node_t n = nodes[id];
        arr.data[i] = n.left;
    }
}

void advance_ids_right(idarr_t arr, node_t* nodes) {
    for (int i = 0; i < arr.size; ++i) {
        id_t id = arr.data[i];
        node_t n = nodes[id];
        arr.data[i] = n.right;
    }
}

bool is_id_end(id_t id) {
    return id % 26 == 25;
}


typedef struct {
    int factors[10];
    int count;
} prime_factors_t;

prime_factors_t get_prime_factors(int num) {
    prime_factors_t fac = {0};
    while (num % 2 == 0) {
        fac.factors[fac.count++] = 2;
        num /= 2;
    };

    for (int i = 3; i < num / 2; i+=2) {
        while (num % i == 0) {
            num /= i;
            fac.factors[fac.count++] = i;
        }
    }

    fac.factors[fac.count++] = num;

    return fac;
}

int count_steps_for_start_id(pd_t pd, id_t start_id) {
    id_t cur_id = start_id;
    int count = 0;
    int ip = 0;
    int instr_count = pd.instructions.size;
    while (!is_id_end(cur_id)) {
        char instr = pd.instructions.data[ip];
        ip = (ip + 1) % instr_count;
        node_t n = pd.nodes[cur_id];
        if (instr == 'L') cur_id = n.left;
        else cur_id = n.right;
        count++;
    }
    return count;
}


bool is_common_factor(prime_factors_t* factor_arr, int fac) {
    prime_factors_t* fptr = factor_arr;
    while (fptr->count) {
        bool has_factor = false;
        for (int i = 0; i < fptr->count; ++i) {
            if (fptr->factors[i] == fac) has_factor = true;
        }
        if (has_factor == false) return false;
        fptr++;
    }
    return true;
}

void reduce_common_factor(prime_factors_t* factor_arr, int fac) {
    prime_factors_t* fptr = factor_arr;
    while (fptr->count) {
        int* gptr = fptr->factors;
        while(*gptr) {
            if (*gptr == fac) {
                *gptr = 1;
                break;
            }
            gptr++;
        }
        fptr++;
    }
}



s64 smallest_common_multiple(prime_factors_t* factor_arr) {
    prime_factors_t* fptr = factor_arr;
    if (fptr->count == 0) return 0;
    while(fptr->count) {
        int* gptr = fptr->factors;
        while(*gptr) {
            if (*gptr != 1) reduce_common_factor(fptr + 1, *gptr);
            gptr++;
        }
        fptr++;
    }
    s64 res = 1;
    fptr = factor_arr;
    while (fptr->count) {
        for (int i = 0; i < fptr->count; ++i) {
            res*=fptr->factors[i];
        }
        fptr++;
    }

    return res;
}

s64 count_steps_part_two(pd_t pd) {
    idarr_t cur_ids = collect_start_ids(pd);
    int* counters = calloc(cur_ids.size, sizeof(int));

    for (int i = 0; i < cur_ids.size; ++i) {
        counters[i] = count_steps_for_start_id(pd, cur_ids.data[i]);
    }

    prime_factors_t* factor_arr = calloc(cur_ids.size, sizeof(prime_factors_t));

    for (int i = 0; i< cur_ids.size; ++i) {
        int count = counters[i];
        prime_factors_t fac = get_prime_factors(count);
        for (int j = 0; j < fac.count; ++j) {
            factor_arr[i] = fac;
        }
    }

    return smallest_common_multiple(factor_arr);
}

s64 part_one(str_t input) {
    pd_t pd = parse_input(input);
    return count_steps(pd);
}

s64 part_two(str_t input) {
    pd_t pd = parse_input(input);
    return count_steps_part_two(pd);
}

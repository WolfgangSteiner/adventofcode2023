#include "aoc.h"
#include "aoc_int_arr.c"
#include <stdarg.h>

typedef struct {
    char* data;
    size_t capacity;
} str_cstr_buffer_t;

static str_cstr_buffer_t str_cstr_buffer = { .data=0, .capacity=0 };
char* _get_cstr_buffer(size_t capacity) {
    capacity = max_s64(capacity, 1024);
    if (str_cstr_buffer.data == 0) {
        str_cstr_buffer.capacity = capacity;
        str_cstr_buffer.data = malloc(str_cstr_buffer.capacity);
    } 
    return str_cstr_buffer.data;
}

str_t str_copy(str_t str) {
    str_t r = {
        .data=malloc(str.size),
        .size=str.size,
        .is_valid=str.is_valid,
        .owns_data=true};
    memcpy(r.data, str.data, str.size);
    return r;
}

bool str_iter_is_end(str_iter_t* iter) {
    return iter->pos >= iter->str->size;
}

bool str_iter_is_rend(str_iter_t* iter) {
    return iter->pos < 0;
}

str_iter_t str_iter_begin(str_t* str) {
    return (str_iter_t){ str, 0 };
}

str_iter_t str_iter_rbegin(str_t* str) {
    return (str_iter_t){ str, str->size - 1};
}

void str_print(str_t str) {
    for (int i = 0; i < str.size; ++i) {
        putchar(str.data[i]);
    }
    putchar('\n');
}

str_t get_line(str_iter_t* iter) {
    str_t result = {0};
    if (str_iter_is_end(iter)) {
        result.is_valid = false;
        return result;
    }
    result.data = iter->str->data + iter->pos;
    result.owns_data = false;
    size_t pos = iter->pos;
    while (pos < iter->str->size) {
        if (iter->str->data[pos] == '\n') {
            result.size = pos - iter->pos;
            iter->pos = pos + 1;
            return result; 
        }
        pos++;
    }
    result.size = pos - iter->pos;
    iter->pos = pos;
    return result;
}

str_t read_file(str_t file_name) {
    str_t result = {0};
    FILE* fp = fopen(str_cstr(file_name), "rb");
    if (fp == 0) return result;
    int fseek_result = fseek(fp, 0L, SEEK_END);
    if (fseek_result) return result;
    result.size = ftell(fp);
    rewind(fp);
    result.data = malloc(result.size);
    fread(result.data, result.size, sizeof(char), fp);
    fclose(fp);
    result.is_valid = true;
    result.owns_data = true;
    return result;
}

bool file_exists(str_t file_name) {
    FILE* file = fopen(str_cstr(file_name), "rb");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

bool str_contains(str_t str, str_t search_str) {
    if (str.size < search_str.size) return false;
    char* cptr = str.data;
    for (int i = 0; i <= str.size - search_str.size; ++i) {
        if (0 == memcmp(cptr++, search_str.data, search_str.size)) return true;
    }
    return false;
}

bool str_contains_char(str_t str, char c) {
    for (int i = 0; i < str.size; ++i) {
        if (str.data[i] == c) return true;
    }
    return false;
}

bool str_eq(str_t a, str_t b) {
    if (a.size != b.size) return false;
    return memcmp(a.data, b.data, a.size) == 0;
}

int char_to_int(char c) {
    return (int)(c - '0');
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_white_space(char c) {
    return c == ' ' || c == '\t';
}

bool is_word_separator(char c) {
    static str_t separators = {.data=" \t,;.:\"'(){}*+~=#|            ", .size=32};
    return str_contains_char(separators, c);
}

bool is_newline(char c) {
    return c == '\n';
}

char str_at(str_t str, int idx) {
    assert(idx < str.size);
    return str.data[idx];
}

int str_to_int(str_t str) {
    int res = 0;
    for (int i = 0; i < str.size; ++i) {
        char c = str_at(str, i);        
        assert(is_digit(c));
        res = 10 * res + char_to_int(c);
    }   
    return res;
}

char str_get_char(str_t str, size_t pos) { return str.data[pos]; }
char str_iter_get_char(str_iter_t* iter) { return iter->str->data[iter->pos]; }
void str_iter_inc(str_iter_t* iter) { iter->pos++; }
void str_iter_dec(str_iter_t* iter) { iter->pos--; }

str_iter_t str_find_char(str_t str, char c) {
    str_iter_t iter = str_iter_begin(&str);
    while (!str_iter_is_end(&iter)) {
        if (str_iter_get_char(&iter) == c) return iter;
        str_iter_inc(&iter);
    }
    return iter;
}

str_t str_split_tail_at_char(str_t str, char c) {
    str_iter_t iter = str_find_char(str, c);
    // the char is not in the string
    if (str_iter_is_end(&iter)) return str_ref("");
    str_iter_inc(&iter);
    // the char is the last char in the string
    if (str_iter_is_end(&iter)) return str_ref("");
    str_t result = {
        .data=str.data + iter.pos,
        .size=str.size - iter.pos,
        .is_valid=true,
        .owns_data=false
    };
    return result;
}

bool str_iter_match(str_iter_t* iter, str_t match_str) {
    if (iter->pos + match_str.size > iter->str->size) return false;
    bool match = (memcmp(iter->str->data + iter->pos, match_str.data, match_str.size) == 0);
    if (match) {
        iter->pos += match_str.size;
        return true;
    } else {
        return false;
    } 
}

bool str_iter_rmatch(str_iter_t* iter, str_t match_str) {
    if (iter->pos < match_str.size) return false;
    bool match = (memcmp(iter->str->data + iter->pos - match_str.size + 1, match_str.data, match_str.size) == 0);
    if (match) {
        iter->pos -= match_str.size;
        return true;
    } else {
        return false;
    } 
}

int str_iter_match_int(str_iter_t* iter) {
    int res = 0;
    if (str_iter_is_white_space(iter)) {
        str_iter_match_white_space(iter);
    }
    while (!str_iter_is_end(iter)) {
        char c = str_iter_get_char(iter);
        if (is_digit(c)) {
            res = 10 * res + char_to_int(c);
            str_iter_inc(iter);
        } else {
          break;
        }
    }    
    return res;
}

s64 str_iter_match_s64(str_iter_t* iter) {
    s64 res = 0;
    if (str_iter_is_white_space(iter)) {
        str_iter_match_white_space(iter);
    }
    while (!str_iter_is_end(iter)) {
        char c = str_iter_get_char(iter);
        if (is_digit(c)) {
            res = 10 * res + char_to_int(c);
            str_iter_inc(iter);
        } else {
          break;
        }
    }    
    return res;
}

bool str_iter_match_white_space(str_iter_t* iter) {
    bool matched = false;
    while (!str_iter_is_end(iter)) {
        char c = str_iter_get_char(iter);
        if (is_white_space(c)) {
            str_iter_inc(iter);
            matched = true;
        } else {
            break;
        }
    }
    return matched;
}

bool str_iter_match_char(str_iter_t* iter, char match) {
    if (str_iter_is_end(iter)) return false;
    if (str_iter_get_char(iter) == match) {
        str_iter_inc(iter);
        return true;
    } else {
        return false;
    }
}

bool str_iter_match_newline(str_iter_t* iter) {
    return str_iter_match_char(iter, '\n');
}

bool str_iter_is_digit(str_iter_t* iter) {
    if (str_iter_is_end(iter)) return false;
    return is_digit(str_iter_get_char(iter));
}

bool str_iter_is_white_space(str_iter_t* iter) {
    if (str_iter_is_end(iter)) return false;
    return is_white_space(str_iter_get_char(iter));
}

str_t str_iter_match_up_to_char(str_iter_t* iter, char match) {
    str_t r = {
        .data=iter->str->data + iter->pos,
        .size=0,
        .is_valid=true,
        .owns_data=false
    };

    while (!str_iter_is_end(iter)) {
        char c = str_iter_get_char(iter);
        str_iter_inc(iter);

        if (c == match) {
            return r;
        } else {
            r.size++;
        }
    }
    return r;
}

str_t str_iter_match_word(str_iter_t* iter) {
    str_t r = {
        .data=iter->str->data + iter->pos,
        .size=0,
        .is_valid=true,
        .owns_data=false
    };

    while (!str_iter_is_end(iter)) {
        char c = str_iter_get_char(iter);
        if (is_word_separator(c)) {
            return r;
        } 
        str_iter_inc(iter);
        r.size++;
    }
    return r;
}

int_arr_t str_iter_match_int_list(str_iter_t* iter) {
    int_arr_t r = int_arr_create();

    while (!str_iter_is_end(iter)) {
        str_iter_match_white_space(iter);
        if (!str_iter_is_digit(iter)) break; 
        s64 v = str_iter_match_s64(iter);
        int_arr_push(&r, v);
    }
    return r;
}


str_t str_ref(char* cstr) {
    return (str_t){ 
        .data=cstr,
        .size=strlen(cstr),
        .is_valid=true,
        .owns_data=false}; 
}

str_t str_new(char* cstr) {
    str_t res = {0};
    size_t len = strlen(cstr);
    res.data = malloc(len);
    res.size = len;
    res.is_valid = true;
    res.owns_data = true;
    memcpy(res.data, cstr, len);
    return res;
}

str_t str_new_with_capacity(size_t capacity) {
    str_t str = {0};
    str.data = calloc(capacity, 1);
    str.is_valid = true; 
    return str;
}

void str_free(str_t str) {
    if (str.owns_data) free(str.data);
}

str_t str_cat(str_t a, str_t b) {
    str_t str = str_new_with_capacity(a.size + b.size);
    str.size = a.size + b.size;
    memcpy(str.data, a.data, a.size);
    memcpy(str.data + a.size, b.data, b.size);
    return str;
}

void str_grow(str_t* str, size_t new_capacity) {
    assert(str->owns_data);
    str->data = realloc(str->data, new_capacity);
}

void str_append(str_t* str, str_t append_str) {
    size_t new_size = str->size + append_str.size;
    str_grow(str, new_size);
    memcpy(str->data + str->size, append_str.data, append_str.size);
    str->size = new_size;
}

void str_append_space(str_t* str) {
    str_append_char(str, ' ');
}

str_t str_from_int(int i) {
    char* buffer = _get_cstr_buffer(32);
    sprintf(buffer, "%d", i);
    return str_new(buffer);
}

str_t str_from_s64(s64 i) {
    char* buffer = _get_cstr_buffer(32);
    sprintf(buffer, "%ld", i);
    return str_new(buffer);
}

str_t str_from_f32(f32 f) {
    char* buffer = _get_cstr_buffer(32);
    sprintf(buffer, "%f", f);
    return str_new(buffer);
}


typedef struct {
    str_t key;
    str_format_callback_t callback;
} str_format_pattern_t;

typedef struct {
    str_format_pattern_t* data;
    size_t size, capacity;
} str_format_pattern_list_t;

static str_format_pattern_list_t str_format_patterns = {0};

str_t _str_format_callback_str(va_list* args, str_t specifier) {
    (void) specifier;
    str_t arg = va_arg(*args, str_t);
    return arg;
}   

str_t _str_format_callback_cstr(va_list* args, str_t specifier) {
    (void) specifier;
    char* arg = va_arg(*args, char*);
    return str_new(arg);
}

str_t _str_format_callback_int(va_list* args, str_t specifier) {
    if (str_eq(specifier, str_ref("hex"))) {
        int arg = va_arg(*args, int);
        char* buffer = _get_cstr_buffer(32);
        sprintf(buffer, "%x", arg);
        return str_new(buffer);
    } else {
        int arg = va_arg(*args, int);
        return str_from_int(arg);
    }
}

str_t _str_format_callback_s64(va_list* args, str_t specifier) {
    if (str_eq(specifier, str_ref("hex"))) {
        s64 arg = va_arg(*args, s64);
        char* buffer = _get_cstr_buffer(32);
        sprintf(buffer, "%lx", arg);
        return str_new(buffer);
    } else {
        s64 arg = va_arg(*args, s64);
        return str_from_s64(arg);
    }
}

str_t _str_format_callback_f32(va_list* args, str_t specifier) {
    f32 arg = (f32)va_arg(*args, double);
    char* buffer = _get_cstr_buffer(32);
    if (!str_eq(specifier, str_ref(""))) {
        str_iter_t iter = str_iter_begin(&specifier);
        int width = str_iter_match_int(&iter);
        str_iter_match_char(&iter, '.');
        int precision = str_iter_match_int(&iter);
        char* fmt_string = malloc(32);
        sprintf(fmt_string, "%%%d.%df", width, precision);
        sprintf(buffer, fmt_string, arg);
        free(fmt_string);
    } else {
        sprintf(buffer, "%.3g", arg);
    }   
    return str_new(buffer);
}

void _str_format_register_pattern(str_t key, str_format_callback_t callback) {
    str_format_patterns.data[str_format_patterns.size++] = (str_format_pattern_t){ key, callback };
}

void _str_format_initialize_pattern_list() {
    static bool first = true;
    if (first) {
        first = false;
        str_format_patterns.capacity = 32;
        str_format_patterns.size = 0;
        str_format_patterns.data = calloc(str_format_patterns.capacity, sizeof(str_format_pattern_t));
        _str_format_register_pattern(str_ref("str"), _str_format_callback_str);
        _str_format_register_pattern(str_ref("cstr"), _str_format_callback_cstr);
        _str_format_register_pattern(str_ref("int"), _str_format_callback_int);
        _str_format_register_pattern(str_ref("s64"), _str_format_callback_s64);
        _str_format_register_pattern(str_ref("f32"), _str_format_callback_f32);
    }
}

str_format_pattern_list_t* _str_format_get_patterns() {
    _str_format_initialize_pattern_list();
    return &str_format_patterns;
}

void _str_format_add_pattern(str_t pattern, str_format_callback_t callback) {
    assert(str_format_patterns.data != 0);
    if (str_format_patterns.capacity == str_format_patterns.size) {
        str_format_patterns.capacity *= 2;
        str_format_patterns.data = realloc(str_format_patterns.data, str_format_patterns.capacity);
    }
    _str_format_register_pattern(pattern, callback);
}

str_format_pattern_t* _str_format_get_pattern_for_key(str_t key) {
    str_format_pattern_list_t* patterns = _str_format_get_patterns();
    for (int i = 0; i < patterns->size; ++i) {
        str_format_pattern_t* pattern = patterns->data + i;
        if (str_eq(key, pattern->key)) {
            return pattern;
        }
    }
    return 0;
}

void str_format_register_pattern(str_t pattern, str_format_callback_t callback) {
    str_format_pattern_t* existing_pattern = _str_format_get_pattern_for_key(pattern);
    if (existing_pattern) {
        existing_pattern->callback = callback;
    } else {
        _str_format_add_pattern(pattern, callback);
    }
}

str_t _str_format_pattern_to_lookup_key(str_t pattern) {
    // turn str:xxx into str
    if (str_contains_char(pattern, ':')) {
        str_iter_t iter = str_iter_begin(&pattern);
        str_t key = str_iter_match_up_to_char(&iter, ':');
        return key;
    } else {
        return pattern;
    }
}

str_t _str_format_pattern_get_format_specifier(str_t pattern) {
    // turn str:xxx into xxx
    if (str_contains_char(pattern, ':')) {
        return str_split_tail_at_char(pattern, ':');
    } else {
        return str_ref("");
    }
}

str_format_callback_t _str_format_get_callback_for_key(str_t key) {
    str_format_pattern_list_t* patterns = _str_format_get_patterns();
    for (int i = 0; i < patterns->size; ++i) {
        str_format_pattern_t* pattern = patterns->data + i;
        if (str_eq(key, pattern->key)) {
            return pattern->callback;
        }
    }
    return 0;
}

str_t str_format(str_t fmt, ...) {
    va_list args;
    va_start(args, fmt);
    // find patterns of form {TYPE}, where type is str, int, s64, f32
    // replace with value of type
    str_iter_t iter = str_iter_begin(&fmt);
    str_t result = str_new("");

    while (!str_iter_is_end(&iter)) {
        if (str_iter_match_char(&iter, '{')) {
            str_t pattern = str_iter_match_up_to_char(&iter, '}');
            str_t key = _str_format_pattern_to_lookup_key(pattern);
            str_t specifier = _str_format_pattern_get_format_specifier(pattern);
            str_format_callback_t callback = _str_format_get_callback_for_key(key);
            if (callback) {
                str_t value = callback(&args, specifier);
                str_append(&result, value);
                str_free(value);
            } else {
                str_append_char(&result, '{');
                str_append(&result, pattern);
                str_append_char(&result, '}');
            }
        } else {
            str_append_char(&result, str_iter_get_char(&iter));
            str_iter_inc(&iter);
        }
    }
    va_end(args);

    return result;
}

void str_append_char(str_t* str, char c) {
    size_t new_size = str->size + 1;
    str_grow(str, new_size);
    str->data[str->size++] = c;
}




char* str_cstr(str_t str) {
    if (str_cstr_buffer.data == 0) {
        str_cstr_buffer.capacity = 1024;
        str_cstr_buffer.data = malloc(str_cstr_buffer.capacity);
    } 
    
    if (str_cstr_buffer.capacity < str.size + 1) {
        while (str_cstr_buffer.capacity < str.size + 1) str_cstr_buffer.capacity *= 2;
        str_cstr_buffer.data = realloc(str_cstr_buffer.data, str_cstr_buffer.capacity);
    }

    memcpy(str_cstr_buffer.data, str.data, str.size);
    str_cstr_buffer.data[str.size] = '\0';
    return str_cstr_buffer.data;
}

char* str_copy_to_cstr(str_t str) {
    char* cstr = calloc(str.size + 1, sizeof(char));
    memcpy(cstr, str.data, str.size);
    return cstr;
}

grid_t grid_from_str(str_t str) {
    str_iter_t iter = str_iter_begin(&str);
    bool is_first_line = true;
    grid_t g = {0};
    while(!str_iter_is_end(&iter)) {
        str_t line = get_line(&iter);
        if (is_first_line) {
            g.width = line.size;     
        }
        grid_add_row(&g, line);
    }

    return g;
}

void grid_add_row(grid_t* g, str_t row) {
    if (g->data == 0) {
        g->data = malloc(1024);
        g->capacity = 1024;
    } else if (g->capacity <= g->width * (g->height + 1)) {
        g->capacity *= 2;
        g->data = realloc(g->data, g->capacity);
    }
    assert(g->width == row.size);
    memcpy(g->data + g->width * g->height, row.data, g->width);
    g->height++;
}

void grid_print(grid_t* g) {
    for (int row = 0; row < g->height; ++row) {
        char* row_ptr = g->data + row * g->height;
        for (int col = 0; col < g->width; ++col) {
            putchar(row_ptr[col]);
        }
        putchar('\n');
    } 
}

char grid_at(grid_t grid, int x, int y) {
    assert(x < grid.width && y < grid.height);
    return grid.data[y*grid.width + x];
}

char* grid_pointer_at(grid_t grid, int x, int y) {
    assert(x < grid.width && y < grid.height);
    return grid.data + y*grid.width + x;
}

s64 pow_s64(int base, int exponent) {
    if (exponent == 0) return 1;

    s64 res = base;
    for (int i = 0; i < exponent - 1; ++i) {
        res*=base;
    }
    return res;
}
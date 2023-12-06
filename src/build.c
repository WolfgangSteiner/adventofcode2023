#include "aoc.h"

str_t path_new(str_t p) {
    return str_copy(p);
}

void path_prepend_dir(str_t* p, str_t dir) {
    assert(p->owns_data);
    str_t new_p = str_copy(dir);
    str_append_char(&new_p, '/');
    str_append(&new_p, *p);
    free(p->data);
    *p = new_p;
}

void path_add_ext(str_t* p, str_t ext) {
    str_append_char(p, '.');
    str_append(p, ext);
}

str_t cmd_new(str_t cmd) {
    return str_copy(cmd);
}

void cmd_append_opt(str_t* cmd, str_t opt, str_t value) {
    str_append_space(cmd);
    str_append(cmd, opt);
    str_append_space(cmd);
    str_append(cmd, value);
}

void cmd_append_arg(str_t* cmd, str_t arg) {
    str_append_space(cmd);
    str_append(cmd, arg);
}

int cmd_run(str_t cmd) {
    return system(str_cstr(cmd));
}

str_t arg_pop(int* argc, char** argv, str_t arg_name) {
    static bool first = true;
    // skip first arg (program name)
    if (first) {
        first = false;
        --*argc;
        ++argv;
    }

    if (*argc < 1) {
        printf("Missing argument: %s\n", str_cstr(arg_name));
        exit(1);
    }    

    str_t arg = str_ref(*argv++);
    (*argc)--;
    return arg;
}

#define ARG_POP(arg_name) arg_pop(&argc, argv, str_ref(arg_name))

int main(int argc, char** argv) {
    str_t day = ARG_POP("day");
    str_t src_file = str_format(str_ref("src/{str}.c"), day);
    str_t out_file = str_format(str_ref("bin/{str}"), day);
    str_t cmd = str_new("gcc -g -Isrc --std=c11 -Wall -lm");
    cmd_append_opt(&cmd, str_ref("-o"), out_file);
    cmd_append_arg(&cmd, src_file);
    cmd_append_arg(&cmd, str_ref("src/aoc.c src/aoc_main.c"));
    int result = cmd_run(cmd);
    if (result == 0) {
        cmd_run(out_file);
    }
}
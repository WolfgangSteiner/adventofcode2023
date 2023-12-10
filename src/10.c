#include "aoc.h"
#include "grid.h"

char* day = "10";
s64 part_one_test_value = 8;
s64 part_two_test_value = 8;


typedef struct {
    int id_a, id_b;
} connection_t;

int connection_get_other_id(connection_t con, int id) {
    assert(con.id_a == id || con.id_b == id);
    return con.id_a == id ? con.id_b : con.id_a;
}

bool connection_has_id(connection_t con, int id) {
    return id == con.id_a || id == con.id_b;
}

connection_t get_connection(grid_t grid, int id) {
    pos_t p = cell_id_to_pos(id);
    char c = grid_at_pos(grid, p) & 0x7f;
    //printf("checking connection for %x(%c)\n", id, c);
    connection_t con = {0};
    switch (c) {
    case '|':
        con.id_a = id - 0x10000;
        con.id_b = id + 0x10000;
        break;
    case '-':
        con.id_a = id - 1;
        con.id_b = id + 1;
        break;
    case 'L':
        con.id_a = id - 0x10000;
        con.id_b = id + 1;
        break;
    case 'J':
        con.id_a = id - 0x10000;
        con.id_b = id - 1;
        break;
    case '7':
        con.id_a = id - 1;
        con.id_b = id + 0x10000;
        break;
    case 'F':
        con.id_a = id + 1;
        con.id_b = id + 0x10000;
        break;
    default:
        con.id_a = 0xffffffff;
        con.id_b = 0xffffffff;
    }

    return con;
}

int cell_id(int x, int y) {
    return (y << 16) + x;
}

pos_t cell_id_to_pos(int id) {
    return (pos_t){.x=id&0xffff, .y=id>>16};
}

typedef struct {
    grid_t* grid;
    int id;
    int prev_id;
    int dist;
    char symbol;
} cell_iter_t;

char cell_iter_get(cell_iter_t* iter) {
    return grid_at_pos(*iter->grid, cell_id_to_pos(iter->id));
}

void cell_iter_mark(cell_iter_t* iter) {
    int x = iter->id & 0xffff;
    int y = iter->id >> 16;
    char c = cell_iter_get(iter);
    c |= 0x80;
    grid_set_cell(*iter->grid, x, y, c);
}

cell_iter_t cell_iter_make(grid_t* g, int prev_id, int id) {
    cell_iter_t iter = { .grid=g, .id=id, .prev_id=prev_id, .dist=1 };
    iter.symbol = cell_iter_get(&iter);
    cell_iter_mark(&iter);
    return iter;
}

void cell_iter_print(cell_iter_t iter) {
    printf("[ITER] id:%x, prev_id:%x, symbol:%c, dist:%d\n", iter.id, iter.prev_id, iter.symbol, iter.dist);
}


void cell_iter_advance(cell_iter_t* iter) {
    connection_t con = get_connection(*iter->grid, iter->id);
    int next_id = connection_get_other_id(con, iter->prev_id);
    iter->prev_id = iter->id;
    iter->id = next_id;
    iter->dist++;
    cell_iter_mark(iter);
}

int find_starting_position(grid_t g) {
    for (int y = 0; y < g.height; ++y) {
        for (int x = 0; x < g.width; ++x) {
            if (grid_at(g, x, y) == 'S') return cell_id(x, y);
        }
    }
    return -1;
}


connection_t find_initial_connection(grid_t g, int start_id) {
    connection_t res = {-1,-1};
    bool found_id_a = false;
    grid_neighbors_t neighbors = grid_neighbors(g, start_id, false); 
    for (int i = 0; i < neighbors.count; i++) {
        connection_t con = get_connection(g, neighbors.arr[i]);
        if (connection_has_id(con, start_id)) {
            int other_id = neighbors.arr[i];
            if (found_id_a == false) {
                res.id_a = other_id;
                found_id_a = true;
            }
            else {
                res.id_b = other_id;
                return res;
            }
        }
    }
    return res;
}

s64 find_max_distance(grid_t g) {
    int start_id = find_starting_position(g);
    connection_t con = find_initial_connection(g, start_id);
    cell_iter_t iter_a = cell_iter_make(&g, start_id, con.id_a);
    cell_iter_t iter_b = cell_iter_make(&g, start_id, con.id_b);

    while (true) {
       cell_iter_advance(&iter_a);
        if (iter_a.id == iter_b.id) {
            return iter_b.dist;
        }
        cell_iter_advance(&iter_b);
        if (iter_a.id == iter_b.id) {
            return iter_a.dist;
        }
    }
    return 0;
}

void mark_loop_cells(grid_t g, int start_id) {
    connection_t con = find_initial_connection(g, start_id);
    cell_iter_t iter_a = cell_iter_make(&g, start_id, con.id_a);
    cell_iter_t iter_b = cell_iter_make(&g, start_id, con.id_b);

    while (true) {
       cell_iter_advance(&iter_a);
        if (iter_a.id == iter_b.id) {
            return;
        }
        cell_iter_advance(&iter_b);
        if (iter_a.id == iter_b.id) {
            return;
        }
    }
}

char get_starting_symbol(int start_id, int a, int b) {
    pos_t p = cell_id_to_pos(start_id);
    pos_t pa = cell_id_to_pos(a);
    pos_t pb = cell_id_to_pos(b);

    if (pa.y == pb.y) return '-';
    if (pa.x == pb.x) return '|';
    if (pa.y == p.y - 1) return pb.x == p.x + 1 ? 'L' : 'J';
    if (pa.y == p.y + 1) return pb.x == p.x + 1 ? 'F' : '7';
    if (pb.y == p.y - 1) return pa.x == p.x + 1 ? 'L' : 'J';
    if (pb.y == p.y + 1) return pa.x == p.x + 1 ? 'F' : '7';
    assert(false);
    return '.';
}


void substitute_starting_symbol(grid_t g, int start_id) {
    connection_t con = find_initial_connection(g, start_id);
    char start_symbol = get_starting_symbol(start_id, con.id_a, con.id_b);
    printf("substituting %c for starting symbol\n", start_symbol);
    pos_t p = cell_id_to_pos(start_id);
    start_symbol |= 0x80;
    grid_set_cell(g, p.x, p.y, start_symbol);
}


int count_enclosed_cells_for_row(char* row, int size) {
    int count = 0;
    bool is_inside = false;
    char span_start = 0;
    for (int i = 0; i < size; ++i) {
        char c = *row++;
        if ((c & 0x80) == 0) {
            if (is_inside) count++;
        } else {
            c = c & 0x7f;
            if (c == '|') is_inside = !is_inside;
            else if (c == 'F' || c == 'L') span_start = c; 
            else if (c == '-');
            else if (c == 'J' && span_start == 'F') is_inside = !is_inside;
            else if (c == '7' && span_start == 'L') is_inside = !is_inside;
        }
    }
    return count;
}

s64 count_enclosed_cells(grid_t g) {
    int start_id = find_starting_position(g);
    mark_loop_cells(g, start_id);
    substitute_starting_symbol(g, start_id);
    int count = 0;
    for (int y = 0; y < g.height; y++) {
        count += count_enclosed_cells_for_row(g.data + y*g.width, g.width);
    }
    return count;
}

s64 part_one(str_t input) {
    grid_t g = grid_from_str(input);
    return find_max_distance(g);
}

s64 part_two(str_t input) {
    grid_t g = grid_from_str(input);
    return count_enclosed_cells(g);
}

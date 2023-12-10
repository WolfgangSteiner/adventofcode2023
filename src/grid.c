#include "grid.h"

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

void grid_set_cell(grid_t grid, int x, int y, char c) {
    char* ptr = grid_pointer_at(grid, x, y);
    *ptr = c;    
}

char* grid_pointer_at(grid_t grid, int x, int y) {
    assert(x < grid.width && y < grid.height);
    return grid.data + y*grid.width + x;
}

void grid_set_visited(grid_t g, int x, int y) {
    char c = grid_at(g, x, y);
    c |= GRID_VISITED_FLAG;
    grid_set_cell(g, x, y, c);
}

grid_neighbors_t grid_neighbors(grid_t g, grid_cell_id_t id, bool include_diagonals) {
    int x = id & 0xffff;
    int y = id >> 16;
    grid_neighbors_t n = {0};
    for (int iy = y - 1; iy <= y + 1; iy++) {
        for (int ix = x - 1; ix <= x + 1; ix++) {
            if (ix == x && iy == y) continue;
            if (ix < 0 || ix >= g.width) continue;
            if (iy < 0 || iy >= g.height) continue;
            if (!include_diagonals && (ix != x && iy != y)) continue;
            n.arr[n.count++] = grid_cell_id_make(ix, iy);
        }
    } 
    return n;
}
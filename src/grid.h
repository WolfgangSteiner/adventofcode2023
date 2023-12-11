#ifndef GRID_H
#define GRID_H
#include "aoc.h"

#define GRID_VISITED_FLAG 0x80;
#define GRID_VISITED_MASK 0x7F;

typedef struct {
    char* data;
    size_t capacity;
    int width, height; } grid_t;

typedef struct {
    int arr[9];
    int count;
} grid_neighbors_t;

typedef struct {
    int x, y;
} pos_t;

typedef u32 grid_cell_id_t; 

grid_t grid_from_str(str_t str);
grid_t grid_new(int width, int height);
grid_t grid_free(grid_t*);

static inline grid_cell_id_t grid_cell_id_make(int x, int y) { return (y << 16) + x; }
grid_neighbors_t grid_neighbors(grid_t g, grid_cell_id_t id, bool include_diagonals);

void grid_add_row(grid_t* g, str_t row);
void grid_print(grid_t* g);
char grid_at(grid_t grid, int x, int y);
static inline char grid_at_pos(grid_t g, pos_t p) { return grid_at(g, p.x, p.y); }
void grid_set_cell(grid_t grid, int x, int y, char c);
void grid_set_visited(grid_t grid, int x, int y);
char* grid_pointer_at(grid_t grid, int x, int y);

#endif
#include "aoc.h"
#include "grid.h"

char* day = "11";
s64 part_one_test_value = 374;
s64 part_two_test_value = 82000210;

typedef struct {
    int x, y;
} galaxy_t;


typedef struct {
    galaxy_t* galaxies;
    size_t num_galaxies;
} pd_t;


void push_galaxy(pd_t* pd, int x, int y) {
    if (pd->galaxies == 0) {
        pd->galaxies = calloc(1024, sizeof(galaxy_t));
    } else if (pd->num_galaxies % 1024 == 0) {
        pd->galaxies = realloc(pd->galaxies, pd->num_galaxies * 2 * sizeof(galaxy_t));
    }

    pd->galaxies[pd->num_galaxies++] = (galaxy_t){x,y};
}

pd_t extract_galaxies(grid_t g, int expansion_factor) {
    pd_t pd = {0};
    int* row_occupancy = calloc(g.width, sizeof(int));
    int* x_offsets = calloc(g.width, sizeof(int));
    int y_offset = 0;
    for (int row = 0; row < g.height; row++) {
        int num_galaxies_in_row = 0;
        for (int col = 0; col < g.width; col++) {
            char c = grid_at(g, col, row);
            if (c == '#') {
                row_occupancy[col] = 1;
                int x = col;
                int y = row + y_offset;
                push_galaxy(&pd, x, y);
                num_galaxies_in_row++;
            }
        }
        if (num_galaxies_in_row == 0) y_offset+=(expansion_factor - 1);
    }

    int x_offset = 0;
    for (int col = 0; col < g.width; col++) {
        x_offsets[col] = x_offset;
        if (row_occupancy[col] == 0) {
            x_offset+=(expansion_factor - 1);
        } 
    }

    for (int i = 0; i < pd.num_galaxies; ++i) {
        galaxy_t* g = pd.galaxies + i;
        int x_offset = x_offsets[g->x];
        g->x += x_offset;        
    }

    free(row_occupancy);
    free(x_offsets);
    return pd;
}

int manhatten_distance(pos_t p1, pos_t p2) {
    int dx = MAX(p1.x, p2.x) - MIN(p1.x, p2.x);
    int dy = MAX(p1.y, p2.y) - MIN(p1.y, p2.y);
    return dx + dy;
}

s64 compute_pairwise_distance_sum(str_t input, s64 expansion_factor) {
    grid_t grid = grid_from_str(input);
    pd_t pd = extract_galaxies(grid, expansion_factor);
    s64 sum = 0;
    for (int i = 0; i < pd.num_galaxies - 1; ++i) {
        galaxy_t g1 = pd.galaxies[i];
        pos_t p1 = { g1.x, g1.y };
        for (int j = i + 1; j < pd.num_galaxies; ++j) {
            galaxy_t g2 = pd.galaxies[j];
            pos_t p2 = { g2.x, g2.y };
            sum += manhatten_distance(p1, p2);
        }
    }
    return sum;
}

s64 part_one(str_t input) {
    return compute_pairwise_distance_sum(input, 2);
}

s64 part_two(str_t input) {
    return compute_pairwise_distance_sum(input, 1000000);
}

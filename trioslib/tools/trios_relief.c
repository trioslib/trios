#include "trios.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void print_usage() {
    printf("Usage: trios_relief xpl_file window_size number_of_prototypes result_path (optional)seed\n");
}

int main(int argc, char *argv[]) {
    xpl_t *xpl;
    window_t *win, *result;
    int win_size;
    int num_prototypes;
    point_weight *pw;
    int seed;
    int i, j, w, h;

    
    
    xpl = xpl_read(argv[1], &win, NULL);
    win_size = atoi(argv[2]);
    num_prototypes = atoi(argv[3]);

    if (argc == 6) {
        seed = atoi(argv[5]);
    } else {
        seed = 0;
    }

    result = window_relief(xpl, win, num_prototypes, win_size, &pw, seed);
    win_write(argv[4], result);
    
    w = win_get_width(win);
    h = win_get_height(win);
    result = win_create(h, w, 1);
    for (i = 0; i < win_size; i++) {
        win_set_point(pw[i].i, pw[i].j, 1, 1, result);
    }
    win_write("test.win", result);
    

    for (i = 0; i < w*h; i++) {
        printf("score(i=%d, j=%d) = %f\n", pw[i].i, pw[i].j, pw[i].weight);
    }

    return 0;
}

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
    int i;

    xpl = xpl_read(argv[1], &win, NULL);
    win_size = atoi(argv[2]);
    num_prototypes = atoi(argv[3]);

    if (argc == 6) {
        seed = atoi(argv[5]);
    } else {
        seed = 0;
    }
    srand(seed);

    pw = reliefBB(xpl, win, num_prototypes);

    result = win_create(win_get_height(win), win_get_width(win), 1);
    for (i = 0; i < win_size; i++) {
        win_set_point(pw[i].i, pw[i].j, 1, 1, result);
    }
    win_write(argv[4], result);

    return 0;
}

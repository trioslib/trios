#ifndef TRIOS_FS_H
#define TRIOS_FS_H

typedef struct {
    float weight;
    int i, j;
} point_weight;

point_weight *reliefBB(xpl_t *xpl, window_t *win, int m);

#endif // TRIOS_FS_H

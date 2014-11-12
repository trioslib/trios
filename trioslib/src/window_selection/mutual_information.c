#include "trios.h"

#define attr(wpat, idx) ((wpat[(idx)/NB] & (1 << (idx)%NB)) != 0 )


int compare_pw(const void *_p1, const void *_p2);/* {
    point_weight *p1, *p2;
    p1 = (point_weight *) _p1;
    p2 = (point_weight *) _p2;
    if (p2->weight == p1->weight) {
        if (p1->i < p2->i ||
            (p1->i == p2->i && p1->j < p2->j) ) 
            return -1;
        return 1;
        /*return p1 - p2;
    } else return p2->weight - p1->weight;
}*/

double MI_R(xpl_BB *node, int var_i, double freq[2][2], double *x0, double *y0) {
    int value;
    
    if (node == NULL) return;
    
    MI_R(node->left, var_i, freq, x0, y0);
    
    value = attr(node->wpat, var_i); /* get the i-th variable value */
    freq[value][0] += node->fq0;
    freq[value][1] += node->fq1;
    
    if (value == 0) *x0 += node->fq0 + node->fq1;
    *y0 += node->fq0;
    
    MI_R(node->right, var_i, freq, x0, y0);
}

double mutual_information_BB(xpl_t *xpl, int var_i) {
    int i, j, k;
    double freq[2][2], x0 = 0, y0 = 0, MI = 0;
    double px, py, pxy;
    freq[0][0] = freq[0][1] = freq[1][0] = freq[1][1] = 0;
    
    MI_R((xpl_BB*)xpl->root, var_i, freq, &x0, &y0);
    x0 = x0 / xpl->sum;
    y0 = y0 / xpl->sum;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            px = (i == 0)? x0 : 1 - x0;
            py = (j == 0)? y0 : 1 - y0;
            pxy = freq[i][j] / xpl->sum;
            MI += pxy * log2(pxy / (px * py));
        }
    }
    return MI;
}

window_t *window_mutual_information(xpl_t *xpl, window_t *domain, int nA, point_weight **_pw) {
    point_weight *pw;
    int i, j, k, n;
    window_t *result;

    n = domain->width * domain->height;
    pw = malloc(sizeof(point_weight) * n);
    for (k = 0; k < n; k++) {
        pw[k].i = k / domain->width;
        pw[k].j = k % domain->width;
        pw[k].weight = mutual_information_BB(xpl, k);
    }   
    qsort(pw, n, sizeof(point_weight), compare_pw);
    for (i = 0; i < n; i++) {
        printf("Point %d %d val %f\n", pw[i].i, pw[i].j, pw[i].weight);
    }
    
    result = win_create(domain->height, domain->width, 1);
    
    for (i = 0; i < nA; i++) {
        win_set_point(pw[i].i, pw[i].j, 1, 1, result);
    }
    
    if (_pw != NULL) {
        *_pw = pw;
    } else {
        free(pw);
    }
    return result;
}
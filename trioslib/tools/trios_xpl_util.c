#include "trios.h"
#include "string.h"
#include "ctype.h"

#include "trios_win_selection.h"

int resample_xpl(xpl_t *domain, window_t *win_domain, window_t *win, char *dest) {
    xpl_t *new_xpl = NULL;
    int n_remove, *to_remove;
    int i, j, k, w, h;

    n_remove = win_get_wsize(win_domain) - win_get_wsize(win);
    trios_malloc(to_remove, sizeof(int) * n_remove, int, "Failed to alloc aux array.");
    w = win_get_width(win_domain);
    h = win_get_height(win_domain);
    /* faz intersecao e monta vetor de remocoes */
    for (i = 0, k = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            if (win_get_point(i, j, 1, win_domain) != 0) {
                if (win_get_point(i, j, 1, win) == 0) {
                    /* removed point */
                    to_remove[k] = i * w + j;
                    k++;
                }
            }
        }
    }

    new_xpl = xpl_remove_variables(domain, to_remove, n_remove);
    xpl_write(dest, new_xpl, win, NULL);
}

point_weight *read_scores(window_t *win_domain, char *score_file) {
    point_weight *scores = NULL;
    double s;
    FILE *f;
    int k, i, j, w, h, n;

    n = win_get_wsize(win_domain);
    w = win_get_width(win_domain);
    h = win_get_height(win_domain);
    scores = malloc(sizeof(point_weight) * n);

    f = fopen(score_file, "r");
    for(k = 0; k < n; k++) {
        fscanf(f, "Point %d %d val %lf\n", &i, &j, &s);
        printf("Scores: i %d j %d  score %f\n", i, j, s);
        scores[k].i = i;
        scores[k].j = j;
        scores[k].weight = s;
    }
    fclose(f);
    return scores;
}

void fill_xpl(xpl_t *domain, window_t *win_domain, point_weight *sorted_scores, char *dest) {
    int i, n, w, *to_remove, *to_remove_iter;
    xpl_t *fill = NULL;
    i = 0;
    n = win_get_wsize(win_domain);
    w = win_get_width(win_domain);
    to_remove = malloc(sizeof(int) * n);

    for (i = 0; i < n; i++) {
        to_remove[i] = sorted_scores[i].i / w + sorted_scores[i].j;
    }

    to_remove_iter = to_remove + n;
    i = 0;
    do {
        if (fill != NULL)
            xpl_free(fill);
        i++;
        to_remove_iter--;
        win_set_point(sorted_scores[n-i].i, sorted_scores[n-i].j, 1, 0, win_domain);
        fill = xpl_remove_variables(domain, to_remove_iter, i);
        printf("I%d N%d Size: %d, n_nodes = %d\n", i, n, n-i, fill->n_nodes);
    } while (i < n && fill->n_nodes != (1 << (n-i) ));

    win_write(dest, win_domain);
}

int main(int argc, char *argv[]) {
    window_t *win, *win_domain;
    xpl_t *domain;
    point_weight *scores;


    if (strcmp(argv[1], "resample_xpl") == 0) {
        domain = xpl_read(argv[2], &win_domain, NULL);
        win = win_read(argv[3]);
        resample_xpl(domain, win_domain, win, argv[4]);
    } else if (strcmp(argv[1], "fill_xpl") == 0) {
        /* usage: fill_xpl xpl variable_score */
        domain = xpl_read(argv[2], &win_domain, NULL);
        scores = read_scores(win_domain, argv[3]);
        fill_xpl(domain, win_domain, scores, argv[4]);
    }

    return 0;
}

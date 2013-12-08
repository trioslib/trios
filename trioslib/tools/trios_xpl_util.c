#include "trios.h"
#include "string.h"
#include "ctype.h"

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

double *read_scores(window_t *win_domain, char *score_file) {
    double *scores = NULL;
    double s;
    FILE *f;
    int k, i, j, w, h, n;


    n = win_get_wsize(win_domain);
    scores = malloc(sizeof(double) * n);

    f = fopen(score_file, "r");

    for(k = 0; k < n; k++) {
        scanf("%d %d %lf", &i, &j, &s);
        printf("Scores: %d %d %f\n", i, j, s);
    }

}

void fill_xpl(xpl_t *domain, window_t *win_domain, double *scores, char *dest) {

}

int main(int argc, char *argv[]) {
    window_t *win, *win_domain;
    xpl_t *domain;
    double *scores;


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

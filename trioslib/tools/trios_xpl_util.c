#include "trios.h"
#include "string.h"
#include "ctype.h"

int main(int argc, char *argv[]) {
    window_t *win_domain, *win;
    xpl_t *domain, *new_xpl;
    int n_remove, *to_remove;
    int i, j, k, w, h;

    domain = xpl_read(argv[1], &win_domain, NULL);
    win = win_read(argv[2]);
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
    printf("n remove %d k %d\n", n_remove, k);
    new_xpl = xpl_remove_variables(domain, to_remove, n_remove);

    xpl_write(argv[3], new_xpl, win, NULL);
    return 0;
}

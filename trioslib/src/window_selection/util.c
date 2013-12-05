#include "trios.h"

#include <stdio.h>

static void visit_nodes_BB(xpl_BB *node, xpl_t *new_xpl, int *idx, int n_remove, unsigned int *wpat) {
    int i, n_tot;
    int bit, shift;
    if (node == NULL) {
        return ;
    }
    for (i = 0; i < new_xpl->wzip; i++) {
        wpat[i] = 0;
    }
    n_tot  = new_xpl->wsize + n_remove;
    for (i = 0; i < n_tot; i++) {
        if (idx[i] != -1) {
            /* colocar o ponto i na posićõa idx[i] do wpat */
            shift = i / NB;
            bit = i % NB;
            if ((node->wpat[shift] & (1 << bit)) != 0) {
                shift = idx[i] / NB;
                bit = idx[i] % NB;
                /*printf("i %d idx %d shift %d wzip %d\n", i, idx[i], shift, new_xpl->wzip);*/
                wpat[shift] = wpat[shift] | (1 << bit);
            }
        }
    }
    xpl_BB_insert(new_xpl, (xpl_BB**)(&new_xpl->root), wpat, node->fq0, node->fq1);

    visit_nodes_BB(node->left, new_xpl, idx, n_remove, wpat);
    visit_nodes_BB(node->right, new_xpl, idx, n_remove, wpat);
}


xpl_t *xpl_remove_variables(xpl_t * old_xpl, int *vars, int n_remove) {
    xpl_t *new_xpl = NULL;
    int *idx = NULL;
    unsigned int *wpat = NULL;
    int i, j, k, n, rem;

    n = old_xpl->wsize;
    trios_malloc(idx, sizeof(int) * n, xpl_t *, "Could not alloc aux array.");

    for (i = 0, k = 0; i < n; i++) {
        rem = 0;
        for (j = 0; j < n_remove; j++) {
            if (vars[j] == i) {
                rem = 1;
                break;
            }
        }
        if (rem == 1) {
            idx[i] = -1;
        } else {
            idx[i] = k;
            k++;
        }
        printf("%d ", idx[i]);
    }
    printf("\n");

    new_xpl = xpl_create(n - n_remove, old_xpl->type);
    trios_malloc(wpat, sizeof(unsigned int) * new_xpl->wzip, xpl_t *, "Could not alloc aux array.");
    visit_nodes_BB((xpl_BB*)old_xpl->root, new_xpl, idx, n_remove, wpat);

    return new_xpl;
}

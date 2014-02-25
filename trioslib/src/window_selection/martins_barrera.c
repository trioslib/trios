#include "trios.h"
#include "math.h"

double log2(double x) {
    return log(x)/log(2);
}

double entropy_bb(xplBB *node, xpl_t *xpl, double alpha) {
    double entr = 0.0;
    double tot;
    
    if (node == NULL) return 0;
    
    tot = node->fq0 + node->fq1;
    entr = (node->fq0/tot) * log2(node->fq0/tot);
    entr += (node->fq1/tot) * log2(node->fq1/tot);
    
    return -entr + entropy_bb(node->left) + entropy_bb(node->right); 
}


double dataset_entropy(xpl_t *sample) {
    
    return 0;
}

window_t *window_martins_barrera(xpl_t *xpl, window_t *domain, point_weight **pw) {
    int n_features, n_total, n_old, stop = 0;
    int *features;
    int i, j, k;
    window_t *win;
    
    n_total = win_get_wsize(domain);
    n_features = n_old = 0;
    win = win_create(domain->height, domain->width, 1);
    trios_malloc(features, sizeof(int) * n_total, "Malloc error");
    
    while (!stop) {
        n_old = n_features;
        
        for (k = 0; k < n_total; k++) {
            i = k / domain->width;
            j = k % domain->width;
            if (win_get_point(win, i, j, 1) == 0) {
                /* tenta adicionar a variavel. Guarda se for melhor que a entropia atual */
            }
        }
        
    }
    
    return win;
}
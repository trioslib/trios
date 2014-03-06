#include "trios.h"
#include "math.h"

double log2(double x) {
    return log(x)/log(2);
}

double entropy_bb(xpl_BB *node, xpl_t *xpl, double alpha) {
    double entr = 0.0;
    double tot;
    
    if (node == NULL) return 0;
    
    tot = node->fq0 + node->fq1;
    entr = (node->fq0/tot) * log2(node->fq0/tot);
    entr += (node->fq1/tot) * log2(node->fq1/tot);
    
    return -entr + entropy_bb(node->left, xpl, alpha) + entropy_bb(node->right, xpl, alpha); 
}

/* adiciona a feature k ao conjunto atual, calcula a entropia, testa contra o mínimo e, se for
melhor, retorna 1*/

xpl_t *try_add_variable(xpl_t *xpl, int *features, int n_tot, int n_features, int k, double max_entropy) {
    int temp;
    double entr;
    xpl_t *new_xpl;
    
    n_features++;
    temp = features[k];
    features[k] = features[n_tot - n_features];
    features[n_tot - n_features] = temp;
    
    new_xpl = xpl_remove_variables(xpl, features, n_tot - n_features);
    entr = entropy_bb((xpl_BB *) xpl->root, xpl, 1);
    
    temp = features[k];
    features[k] = features[n_tot - n_features];
    features[n_tot - n_features] = temp;
    
    xpl_free(xpl);
    return 0;
}

window_t *window_martins_barrera(xpl_t *xpl, window_t *domain, point_weight **pw) {
    int n_features, n_total, n_old, stop = 0;
    /* features contains the unused features in the left and the selected ones in the right
     *   ______________________ n_tot - n_feat______n_tot
     *  |_______unused__________________|__used_____|
     *
     *  features can be passed to xpl_remove_variables. 
     */
    
    int *features;
    int i, j, k;
    window_t *win;
    double current_entropy, best_entropy;
    
    n_total = win_get_wsize(domain);
    n_features = n_old = 0;
    win = win_create(domain->height, domain->width, 1);
    trios_malloc(features, sizeof(int) * n_total, window_t *, "Malloc error");
    
    while (!stop) {
        n_old = n_features;
        
        best_entropy = current_entropy;
        
        for (k = 0; k < n_total - n_features; k++) {
            i = k / domain->width;
            j = k % domain->width;
            if (win_get_point(i, j, 1, win) == 0) {
                /* calcula a entropia do conjunto atual + k
                 * se é melhor que a entropia atual, guarda, se não joga fora
                 */
                try_add_variable(xpl, features, n_total, n_features, k, best_entropy);
                features[n_features] = k;
                n_features++;
            }
        }
        /* adiciona a variavel com melhor entropia. se nenhuma variavel melhora o conjunto atual pare.*/
        
        if (best_entropy == current_entropy) break;
        else n_features++;
        
    }
    
    return win;
}
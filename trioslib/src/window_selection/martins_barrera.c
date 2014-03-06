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
    entr = 0.0;
    if (node->fq0 > 0) {
        entr += (node->fq0/tot) * log2(node->fq0/tot);
    }
    if (node->fq1 > 0) {
        entr += (node->fq1/tot) * log2(node->fq1/tot);
    }
    
    entr = -entr * (tot + alpha);
    
    return entr + entropy_bb(node->left, xpl, alpha) + entropy_bb(node->right, xpl, alpha); 
}

double try_add_variable(xpl_t *xpl, int *features, int n_tot, int n_features, int k) {
    int temp;
    double entr;
    xpl_t *new_xpl;
    
    n_features++;
    temp = features[k];
    features[k] = features[n_tot - n_features];
    features[n_tot - n_features] = temp;
    
    new_xpl = xpl_remove_variables(xpl, features, n_tot - n_features);
    entr = entropy_bb((xpl_BB *) new_xpl->root, new_xpl, 1);
    entr += (2 << n_features) + xpl->sum;
    
    temp = features[k];
    features[k] = features[n_tot - n_features];
    features[n_tot - n_features] = temp;
    
    xpl_free(new_xpl);
    return entr;
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
    int i, j, k, temp;
    window_t *win;
    double current_entropy, best_entropy, entropy;
    best_entropy = current_entropy = 1000000000000; /* infinity */
    
    n_total = win_get_wsize(domain);
    n_features = n_old = 0;
    win = win_create(domain->height, domain->width, 1);
    trios_malloc(features, sizeof(int) * n_total, window_t *, "Malloc error");
    
    for (i = 0; i < n_total; i++) features[i] = i;
    
    printf("Starting martins_barrera algorithm\n");
    while (!stop && n_features < n_total) {
        printf("Iteration start\n");
        n_old = n_features;
        
        best_entropy = current_entropy;
        
        for (k = 0; k < n_total - n_features; k++) {
            i = k / domain->width;
            j = k % domain->width;
            if (win_get_point(i, j, 1, win) == 0) {
                
                entropy = try_add_variable(xpl, features, n_total, n_features, k);
                /*printf("Entropy %f\n", entropy);*/
                if (entropy < best_entropy) {
                    best_entropy = entropy;
                    /* troca k por n_tot -  n_features - 1 */
                    temp = features[k];
                    features[k] = features[n_total - n_features - 1];
                    features[n_total - n_features - 1] = temp;
                }
                
            }
        }
        printf("Best variable: %d Entropy %f Current entropy %f\n", features[n_total - n_features - 1], best_entropy, current_entropy);
        if (best_entropy >= current_entropy) {
            /* time to stop adding variables */
            break;
        } else {
            n_features++;
            current_entropy = best_entropy;
        }
    }
    
    printf("Num selected %d\n:", n_features);
    for (k = 1; k <= n_features; k++) {
        i = features[n_total - k] / domain->width;
        j = features[n_total - k] % domain->width;
        win_set_point(i, j, 1, 1, win);

        printf("%d ", features[n_total - k]);        
    }
    printf("\n");
    
    return win;
}
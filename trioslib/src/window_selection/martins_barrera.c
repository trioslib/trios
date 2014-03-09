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
    entr += (2 << n_features) - new_xpl->n_nodes;
    
    temp = features[k];
    features[k] = features[n_tot - n_features];
    features[n_tot - n_features] = temp;
    
    xpl_free(new_xpl);
    return entr;
}

double try_remove_variable(xpl_t *xpl, int *features, int n_tot, int n_features, int k) {
    int temp;
    double entr;
    xpl_t *new_xpl;
    
    temp = features[k];
    features[k] = features[n_tot - n_features];
    features[n_tot - n_features] = temp;
    
    new_xpl = xpl_remove_variables(xpl, features, n_tot - n_features + 1);
    entr = entropy_bb((xpl_BB *)new_xpl->root, new_xpl, 1);
    entr += (2 << (n_features-1)) - new_xpl->n_nodes;
    
    temp = features[k];
    features[k] = features[n_tot - n_features];
    features[n_tot - n_features] = temp;
    
    return entr;
    
}

/*Returns the best_entropy found or -1 if the no variable decreases the entropy.
 * If a variable is selected, n_features_ decreases.*/
double sequential_backward_selection_step(xpl_t *xpl, window_t *domain, int *features, int n_total, int *n_features_, double best_entropy) {
    int k, temp;
    int remove = 0;
    int n_features;
    double entropy;
    
    n_features = *n_features_;
    if (n_features == 1) return -1;
    
    for (k = n_total - n_features; k < n_total; k++) {
        entropy = try_remove_variable(xpl, features, n_total, n_features, k);
        printf("Entropy %f best %f\n", entropy, best_entropy);
        if (entropy < best_entropy) {
            remove = 1;
            temp = features[n_total - n_features];
            features[n_total - n_features] = features[k];
            features[k] = temp;
        }
    }
    
    if (remove) {
        printf("Removing variable %d decreases entropy.\n", features[n_total - *n_features_]);
        *n_features_ -= 1;
        return best_entropy;
    } else {
        return -1; 
    }
}

/*Returns the best_entropy found or -1 if the no variable decreases the entropy.
 * If a variable is selected, n_features increases and it is put in features[n_total - n_features].*/
double sequential_forward_selection_step(xpl_t *xpl,  window_t *domain,  int *features, int n_total, int *n_features_, double best_entropy) {
    int k, temp;
    int n_features;
    double entropy;
    n_features = *n_features_;
    int add_var = 0;
    for (k = 0; k < n_total - n_features; k++) {        
        entropy = try_add_variable(xpl, features, n_total, n_features, k);
        /*printf("Entropy %f\n", entropy);*/
        if (entropy < best_entropy) {
            add_var = 1;
            best_entropy = entropy;
            /* troca k por n_tot -  n_features - 1 */
            temp = features[k];
            features[k] = features[n_total - n_features - 1];
            features[n_total - n_features - 1] = temp;
        }
    }
    if (add_var) {
        *n_features_ += 1;
        printf("Adding variable %d decreases entropy.\n", features[n_total - *n_features_]);
        return best_entropy;
    } else {
        return -1;
    }
}


window_t *window_martins_barrera(xpl_t *xpl, window_t *domain, point_weight **pw) {
    /* features contains the unused features in the left and the selected ones in the right
     *   ______________________ n_tot - n_feat______n_tot
     *  |_______unused__________________|__used_____|
     *
     *  features can be passed to xpl_remove_variables. 
     */
    int *features;
    int n_features, n_total, stop = 0;
    int i, j, k, temp;
    int noadd, noremove;
    window_t *win;
    double current_entropy, best_entropy, entropy;
    best_entropy = current_entropy = 1000000000000; /* infinity */
    
    n_total = win_get_wsize(domain);
    n_features = 0;
    win = win_create(domain->height, domain->width, 1);
    trios_malloc(features, sizeof(int) * n_total, window_t *, "Malloc error");
    
    for (i = 0; i < n_total; i++) features[i] = i;
    
    while (!stop && n_features < n_total) {
        printf("Iteration start\n");
        noadd = noremove = 0;
        /* do a SFS step */        
        best_entropy = sequential_forward_selection_step(xpl, domain, features, n_total, &n_features, current_entropy);
        if (best_entropy >= current_entropy || best_entropy == -1) {
            noadd = 1;
        } else {
            current_entropy = best_entropy;
        }
        
        best_entropy = sequential_backward_selection_step(xpl, domain, features, n_total, &n_features, current_entropy);
        if (best_entropy >= current_entropy || best_entropy == -1) {
            noremove = 1;
        } else {
            current_entropy = best_entropy;
        }
        if (noadd && noremove) {
            break;
        }
        printf("Iteration end. Entropy %f\n", current_entropy);
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
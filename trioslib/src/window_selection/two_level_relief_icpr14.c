#include "trios.h"

int xpl_order(xpl_BB *x1, unsigned int **order, int i, int wzip) {
    int j;
    if (x1 == NULL) return i;
    
    i = xpl_order(x1->left, order, i, wzip);
    
    for (j = 0; j < wzip; j++) {
        order[i][j] = x1->wpat[j];
    }
    
    return xpl_order(x1->right, order, i+1, wzip);
}

void compare_xpl_R(xpl_BB* x1, xpl_BB* x2, int wzip) {
    int i, j;
    
    if (x1 == NULL) return;
    if (x2 == NULL) return;
    
    compare_xpl_R(x1->left, x2->left, wzip);
    
    for (i = 0; i < wzip; i++) {
        if (x1->wpat[i] != x2->wpat[i]) {
            printf("Padrão diferenteeeeeeeee\n\n\n");
            
            for (j = 0; j < wzip; j++) {
                printf("%x ", x1->wpat[j]);
            }
            printf("\n");
            for (j = 0; j < wzip; j++) {
                printf("%x ", x2->wpat[j]);
            }
            exit(-1);
        }
    }
    
    compare_xpl_R(x1->right, x2->right, wzip);
}

void compare_xpl(xpl_t *xpl1, xpl_t *xpl2) {
    unsigned int **order1, **order2;
    int i, j;
    
    if (xpl1->n_nodes != xpl2->n_nodes) printf("n_nodes %d %d\n", xpl1->n_nodes, xpl2->n_nodes);
    if (xpl1->sum != xpl2->sum) printf("sum %d %d\n", xpl1->sum, xpl2->sum);
    
    order1 = malloc(sizeof(unsigned int *) * xpl1->n_nodes);
    order2 = malloc(sizeof(unsigned int *) * xpl2->n_nodes);
    for (i = 0; i < xpl1->n_nodes; i++) {
        order1[i] = malloc(sizeof(unsigned int) * xpl1->wzip);
        order2[i] = malloc(sizeof(unsigned int) * xpl2->wzip);
    }
    
    xpl_order((xpl_BB*)xpl1->root, order1, 0, xpl1->wzip);
    xpl_order((xpl_BB*)xpl2->root, order2, 0, xpl2->wzip);
    
    for (i = 0; i < xpl1->n_nodes; i++) {
        for (j = 0; j < xpl1->wzip; j++) {
            if (order1[i][j] != order2[i][j]) {
		printf("Elemento %d: \n", i);	
                for (j = 0; j < xpl1->wzip; j++) {
                    printf("%x ",order1[i][j]);
                }
                printf("\n");
                for (j = 0; j < xpl2->wzip; j++) {
                    printf("%x ", order2[i][j]);
                }
                exit(-1);
                
            }
        }
    }
    
    
    
    /*compare_xpl_R((xpl_BB *)xpl1->root, (xpl_BB *)xpl2->root, xpl1->wzip);*/
}

int select_number_of_first_level_operators(xpl_t *xpl2nd, point_weight *ranking, int n_windows, xpl_t **level2_new) {
    int i, *to_remove, *to_remove_iter;
    xpl_t *temp = NULL;
    
    trios_malloc(to_remove, sizeof(int) * n_windows, int, "Failed to alloc array");
    for (i = 0; i < n_windows; i++) {
        to_remove[i] = ranking[i].j;
    }
    
    to_remove_iter = to_remove + n_windows;
    i = 0;
    do {
        if (temp != NULL) xpl_free(temp);
        to_remove_iter--;
        i++;
        temp = xpl_remove_variables(xpl2nd, to_remove_iter, i);
    } while (i < n_windows && temp->n_nodes != ( 1 << (n_windows - i) ) );
    *level2_new = temp;
    
    return n_windows - i;
}

multi_level_operator_t *window_two_level_relief(window_t *domain, imgset_t *T1, imgset_t *T2, int n_1st_level, int sz_1st_level, int n_proto1, int n_proto2, int type) {
    image_operator_t **ops, *temp_op;
    multi_level_operator_t *result;
    window_t **wins;
    window_t *temp, *temp2;
    xpl_t *xpl_domain, *level2, *level2_new;
    point_weight *operator_ranking;
    int i, j, num1stlevel;
    char cmd[256];
    
    trios_malloc(ops, sizeof(image_operator_t *) * n_1st_level, window_t **, "Failed to alloc operator vector\n");
    trios_malloc(wins, sizeof(window_t) * n_1st_level, window_t **, "Failed to alloc window vector\n");
    xpl_domain = lcollec_memory(T1, domain, type);
    
    for (i = 0; i < n_1st_level; i++) {
        wins[i] = window_relief(xpl_domain, domain, n_proto1, sz_1st_level, NULL, i*100 + i);
        sprintf(cmd, "win-%d", i);
        win_write(cmd, wins[i]);
    }
    
    for (i = 0; i < n_1st_level; i++) {
        if (type == BB) {
            ops[i] = image_operator_build_bb(T1, wins[i]);
        } else if (type == GB) {
            ops[i] = image_operator_build_gb(T1, wins[i]);
        } else if (type == GG) {
            ops[i] = image_operator_build_gg(T1, wins[i]);
        } else {
            trios_fatal("Operator type not supported.");
        }
        sprintf(cmd, "op-%d", i);
        image_operator_write(cmd, ops[i]);
    }
    
    level2 = lcollec_two_level(T2, ops, n_1st_level);
    
    temp = win_create(1, n_1st_level, 1);
    for (i = 0; i < n_1st_level; i++) {
        win_set_point(1, i, 1, 1, temp);
    }
    
    xpl_write("xpl2ndlevel", level2, temp, NULL);
    
    temp2 = window_relief(level2, temp, n_proto2, 1, &operator_ranking, 0);
    
    num1stlevel = select_number_of_first_level_operators(level2, operator_ranking, n_1st_level, &level2_new);
    
    /* faz lista dos operadores de primeiro nível
     * manda para o multi_level_combine_xpl
     * devolve o operador dois níveis treinado.
     *
     * esperamos que ele seja igual ao treinado usando as etapas individuais :) */
    
    /* put all selected operators in the first num1stlevel positions of ops */
    for (i = 0; i < num1stlevel; i++) {
        temp_op = ops[i];
        ops[i] = ops[ operator_ranking[i].j ];
        ops[ operator_ranking[i].j ] = temp_op;
    }
    for (i = num1stlevel; i < n_1st_level; i++) {
        image_operator_free(ops[i]);
    }
    
    result = multi_level_combine_xpl(level2_new, ops, num1stlevel);
    
    
    free(ops);
    free(wins);
    win_free(temp);
    win_free(temp2);

    return result;
}
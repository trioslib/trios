#include "trios.h"



window_t **window_two_level_relief(window_t *domain, imgset_t *T1, imgset_t *T2, int n_1st_level, int sz_1st_level, int n_proto1, int n_proto2, int type) {
    image_operator_t **ops;
    window_t **wins;
    window_t *temp, *temp2;
    xpl_t *xpl_domain, level2;
    point_weight *operator_ranking;
    int i, j;
    
    char cmd[256];
    
    trios_malloc(ops, sizeof(image_operator_t *) * n_1st_level, window_t **, "Failed to alloc operator vector\n");
    trios_malloc(wins, sizeof(window_t) * n_1st_level, window_t **, "Failed to alloc window vector\n");
    xpl_domain = lcollec_memory(T1, domain, type);
    
    for (i = 0; i < n_1st_level; i++) {
        wins[i] = window_relief(xpl_domain, domain, n_proto1, sz_1st_level, NULL, i*10 + i);
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
    /*
    level2 = lcollec_two_level(T2, ops, n_1st_level);
    temp = win_create(1, n_1st_level, 1);
    for (i = 0; i < n_1st_level; i++) {
        win_set_point(1, i, 1, 1, temp);
    }
    temp2 = window_relief(level2, temp, n_proto2, 1, &operator_ranking, 0);*/
    
    /* ordena os operadores por point_weight. */
    /* executa algoritmo combination size 
    
      executa two_level_collec nos operadores
      executa o relief no xpl do segundo nivel
      executa o combination size para encontrar o tamanho correto
    
      devolve as janelas treinadas (devolve também os operadores? devolve direto o operador dois níveis?)
    */
    
    
    free(ops);
    free(wins);
    win_free(temp);
    win_free(temp2);

    return NULL;
}
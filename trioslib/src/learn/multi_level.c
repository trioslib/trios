#include <trios_multi.h>

#include <stdlib.h>

multi_level_operator_t *multi_level_build(multi_architecture_t *m, imgset_t *set) {
    int i, j, k;
    multi_level_operator_t *op = multi_level_operator_create(m);


    for (i = 0; i < op->nlevels; i++) {
        for (j = 0; j < op->levels[i].noperators; j++) {
            /* faz collec em cada um dos operadores */
            /* decision em cada um dos operadores */
            /* isi em cada um dos operadores = wait forever */
            /* aplica cada um dos operadores do nível e passa para o próximo nível treinar */
        }
    }
    return op;
}

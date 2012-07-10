#include <trios_multi.h>

#include <stdlib.h>

multi_level_operator_t *multi_level_build(multi_architecture_t *m, imgset_t *set) {
    int i, j, k;
    multi_level_operator_t *op = multi_level_operator_create(m);

    img_t **input_images;
    img_t **ideal_images;
    for (i = 0; i < op->nlevels; i++) {
        for (j = 0; j < op->levels[i].noperators; j++) {
            /* cria janela aglutinada */
            /* faz collec em cada um dos operadores */
            xpl_t * op_collec = NULL;
            /* decision em cada um dos operadores */
            /* isi em cada um dos operadores = wait forever */
            /* aplica cada um dos operadores do nível e passa para o próximo nível treinar */
        }
    }
    return op;
}

#include <trios_multi.h>
#include <trios_error.h>
#include <stdlib.h>

img_t *multi_level_apply_level(multi_level_operator_t *mop, int level, int op, img_t **inputs) {
    /* roteiro
     *
     *  1. Passeia pelas imagens de input.
     *  2. Monta a janela.
     *  3. Consulta operador
     *  4. Coloca resultado na saída.
     *
     */
    img_t *output;
    int i, j, k, l, m;
    int w, h, win_size = 0;
    int *w_pattern;
    w = img_get_width(inputs[0]);
    h = img_get_height(inputs[0]);

    for (i = 0; i < mop->levels[level].ninputs; i++) {
        win_size += win_get_wsize(mop->levels[level].windows[op][i]);
    }

    output = img_create(w, h, 1, img_get_pixel_size(inputs[0]));
    trios_malloc(w_pattern, sizeof(int) * win_size, "Bad alloc");
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            for (l = 0; l < win_size; l++) {
                w_pattern[l] = 0;
            }
            for (l = 0; l < mop->levels[level].ninputs; l++) {

            }
        }
    }

    return output;
}

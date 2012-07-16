#include <trios_multi.h>
#include <trios_error.h>
#include <trios_io.h>
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
    int w, h, win_size = 0, curr_off;
    int npixels;
    int *w_pattern, *offset;
    w = img_get_width(inputs[0]);
    h = img_get_height(inputs[0]);
    npixels = w * h;
    for (i = 0; i < mop->levels[level].ninputs; i++) {
        win_size += win_get_wsize(mop->levels[level].windows[op][i]);
    }

    output = img_create(w, h, 1, img_get_pixel_size(inputs[0]));
    trios_malloc(w_pattern, sizeof(int) * size_of_zpat(win_size), "Bad alloc");
    trios_malloc(offset, sizeof(int) * win_size, "Bad alloc");
    for (i = 0; i < npixels; i++) {
        for (l = 0; l < size_of_zpat(win_size); l++) {
            w_pattern[l] = 0;
        }
        curr_off = 0;
        for (l = 0; l < mop->levels[level].ninputs; l++) {
            offset_set(offset, multi_level_operator_get_window(mop, level, op, l), img_get_width(inputs[l]), 1);
            for (m = 0; m < win_get_width(mop->levels[level].windows[op][l]); m++) {
                int img_idx = i + offset[m];
                if (img_idx >= 0 && img_idx < npixels && output->data[img_idx] != 0) {
                    int idx, bit;
                    idx = (m + curr_off) / NB;
                    bit = (m + curr_off) % NB;
                    w_pattern[idx] = w_pattern[idx] | bitmsk[bit];
                }
                curr_off += win_get_wsize(mop->levels[level].windows[op][l]);
            }
        }
        /* look for w_pattern in intervals */
        int label = itv_list_contain(mop->levels[level].trained_operator[op], w_pattern, size_of_zpat(win_size));
        output->data[i] = (unsigned char) label;
    }

    free(offset);

    return output;
}

img_t *multi_level_apply(multi_level_operator_t *op, img_t *img) {
    return NULL;
}

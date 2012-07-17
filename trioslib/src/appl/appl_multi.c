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

    if (!itv_label(mop->levels[level].trained_operator[op], 1, 255)) {
        return trios_error(MSG, "lpapplic: itv_label() failed.");
    }

    for (i = 0; i < mop->levels[level].ninputs; i++) {
        win_size += win_get_wsize(mop->levels[level].windows[op][i]);
    }

    output = img_create(w, h, 1, img_get_pixel_size(inputs[0]));
    trios_malloc(w_pattern, sizeof(int) * size_of_zpat(win_size), "Bad alloc");
    trios_malloc(offset, sizeof(int) * win_size, "Bad alloc");

    if (level == 1) {
        char fl[200];
        for (i = 0; i < mop->levels[level].ninputs; i++) {
            sprintf(fl, "inputl1op%d.pgm", i);
            img_writePGM(fl, inputs[i]);
        }
    }

    for (k = 0; k < npixels; k++) {
        for (l = 0; l < size_of_zpat(win_size); l++) {
            w_pattern[l] = 0;
        }
        curr_off = 0;
        for (i = 0; i < mop->levels[level].ninputs; i++) {
            offset_set(offset, multi_level_operator_get_window(mop, level, op, i), img_get_width(inputs[i]), 1);
            for (j = 0; j < win_get_wsize(mop->levels[level].windows[op][i]); j++) {
                l = k + offset[j];
                if (l >= 0 && l < npixels && img_get_pixel(inputs[i], l / inputs[i]->width, l % inputs[i]->width, 0) != 0) {
                    int idx, bit;
                    idx = (j + curr_off) / NB;
                    bit = (j + curr_off) % NB;
                    w_pattern[idx] = (w_pattern[idx] | bitmsk[bit]);
                }
            }
            curr_off += win_get_wsize(mop->levels[level].windows[op][i]);
        }
        /* look for w_pattern in intervals */
        int label = itv_list_contain(mop->levels[level].trained_operator[op], w_pattern, size_of_zpat(win_size));
        img_set_pixel(output, k / output->width, k % output->width, 0, (unsigned char) label);
    }

    free(offset);

    return output;
}

img_t *multi_level_apply(multi_level_operator_t *op, img_t *img) {
    return NULL;
}

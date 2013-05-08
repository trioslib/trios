#include <trios_multi.h>
#include <trios_error.h>
#include <trios_io.h>
#include <stdlib.h>

#include "../collec/collec_private.h"

img_t *multi_level_apply_level_bb(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask) {
    img_t *output;
    int i, j, k, l, m;
    int w, h, win_size = 0, curr_off;
    int npixels;
    int *w_pattern, *offset;
    w = img_get_width(inputs[0]);
    h = img_get_height(inputs[0]);
    npixels = w * h;

    if (!itv_label((itv_t *) mop->levels[level].trained_operator[op], 1, 255)) {
        return (img_t *) trios_error(MSG, "lpapplic: itv_label() failed.");
    }

    for (i = 0; i < mop->levels[level].ninputs; i++) {
        win_size += win_get_wsize(mop->levels[level].windows[op][i]);
    }

    output = img_create(w, h, 1, img_get_pixel_size(inputs[0]));
    trios_malloc(w_pattern, sizeof(int) * size_of_zpat(win_size), img_t *, "Bad alloc");
    trios_malloc(offset, sizeof(int) * win_size, img_t *, "Bad alloc");

    if (level == 1) {
        char fl[200];
        for (i = 0; i < mop->levels[level].ninputs; i++) {
            sprintf(fl, "inputl1op%d.pgm", i);
            img_writePGM(fl, inputs[i]);
        }
    }

    for (k = 0; k < npixels; k++) {
        if (mask != NULL && img_get_pixel(mask, k / mask->width, k % mask->width, 0) == 0) {
            continue;
        }
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
        int label = itv_list_contain((itv_t *)mop->levels[level].trained_operator[op], w_pattern, size_of_zpat(win_size));
        img_set_pixel(output, k / output->width, k % output->width, 0, (unsigned char) label);
    }

    free(offset);
    free(w_pattern);
    return output;
}

img_t *multi_level_apply(multi_level_operator_t *mop, img_t *img, img_t *mask) {
    img_t **input, **next, *result;
    int i, j, k;
    input = &img;

    trios_malloc(next, sizeof(img_t *) * mop->levels[0].noperators, img_t *, "Bad alloc");
    for (i = 0; i < mop->nlevels; i++) {
        for (j = 0; j < mop->levels[i].noperators; j++) {
            next[j] = multi_level_apply_level_bb(mop, i, j, input, mask);
        }
        if (i > 0) {
            for (j = 0; j < mop->levels[i].ninputs; j++) {
                img_free(input[j]);
            }
            free(input);
        }
        input = next;
        if (i < mop->nlevels - 1) {
            trios_malloc(next, sizeof(img_t *) * mop->levels[i].noperators, img_t *, "Bad alloc");
        }
    }
    result = input[0];
    free(input);
    return result;
}

#include <trios_multi.h>

#include <stdlib.h>
#include <stdio.h>
#include "trios_common.h"


xpl_t *collec_level_operator_bb_main(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask, img_t *ideal);
xpl_t *collec_level_operator_gg_main(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask, img_t *ideal);

xpl_t *lcollec_multi_level(multi_level_operator_t *mop, int level, int op, img_t ***input, img_t **mask, img_t **ideal, int n_images) {
    xpl_t *xpl = NULL, *xpl_new = NULL;
    int i;
    if (mop == NULL) {
        return (xpl_t *) trios_error(MSG, "Operator pointer is null");
    }
    if (level >= mop->nlevels) {
        return (xpl_t *) trios_error(MSG, "Operator has less than %d levels", level);
    }
    if (op >= mop->levels[level].noperators) {
        return (xpl_t *) trios_error(MSG, "Level %d contains less than %d operators.", op);
    }

    for (i = 0; i < n_images; i++) {
        if (mop->type == BB) {
            xpl_new = collec_level_operator_bb_main(mop, level, op, input[i], mask[i], ideal[i]);
        } else {
            xpl_new = collec_level_operator_gg_main(mop, level, op, input[i], mask[i], ideal[i]);
        }
        if (xpl == NULL) {
            xpl = xpl_new;
            xpl_new = NULL;
        } else if (xpl_new != NULL) {
            if (!xpl_merge(xpl_new, xpl)) {
                return (xpl_t *) trios_error(MSG, "multi-level collec: merge error");
            }
            xpl_free(xpl);
            xpl = xpl_new;
            xpl_new = NULL;
        }
    }

    return xpl;
}

xpl_t *collec_level_operator_bb_main(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask, img_t *ideal) {
    int i, j, k, l, npixels, win_size = 0;

    for (i = 0; i < mop->levels[level].ninputs; i++) {
        win_size += win_get_wsize(mop->levels[level].windows[op][i]);
    }
    xpl_t *xpl = xpl_create(win_size, BB);
    int *joint_wpat = malloc(sizeof(int) * size_of_zpat(win_size));
    int *offset = malloc(sizeof(int) * win_size);
    /* pega o tamanho das imagens */
    npixels = img_get_width(inputs[0]) * img_get_height(inputs[0]);

    for (k = 0; k < npixels; k++) {
        /* varre todo pixel */
        for (l = 0; l < size_of_zpat(win_size); l++) {
            joint_wpat[l] = 0;
        }
        int curr_win_size = 0; /* number of points of the previous windows. */
        /* mask condition */
        if (mask != NULL && img_get_pixel(mask, k / mask->width, k % mask->width, 0) == 0) {
            continue;
        }
        for (i = 0; i < mop->levels[level].ninputs; i++) {
            offset_set(offset, multi_level_operator_get_window(mop, level, op, i), img_get_width(inputs[i]), 1);
            for (j = 0; j < win_get_wsize(mop->levels[level].windows[op][i]); j++) {
                l = offset[j] + k;
                if (l >= 0 && l < npixels) {
                    if (img_get_pixel(inputs[i], l / inputs[i]->width, l % inputs[i]->width, 0) != 0) {
                        int index = (j + curr_win_size)/NB;
                        int bit = (j + curr_win_size) % NB;
                        joint_wpat[index] = (joint_wpat[index] | bitmsk[bit]);
                    }
                }
            }
            curr_win_size += win_get_wsize(mop->levels[level].windows[op][i]);
        }
        /* insere na arvore. */
        if (ideal->data[k] == 0) {
            xpl_BB_insert(xpl, (xpl_BB **) &xpl->root, joint_wpat, 1, 0);
        } else {
            xpl_BB_insert(xpl, (xpl_BB **) &xpl->root, joint_wpat, 0, 1);
        }
    }
    free(offset);
    free(joint_wpat);
    return xpl;
}


xpl_t *collec_level_operator_gg_main(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask, img_t *ideal) {
    int i, j, k, l, npixels, w, h, win_size = 0, *joint_wpat, *offset, win_offset;
    xpl_t *xpl;
    freq_node *freq;

    for (i = 0; i < mop->levels[level].ninputs; i++) {
        win_size += win_get_wsize(mop->levels[level].windows[op][i]);
    }
    xpl = xpl_create(win_size, GG);
    joint_wpat = malloc(sizeof(int) * win_size);
    offset = malloc(sizeof(int) * win_size);

    w = img_get_width(inputs[0]);
    h = img_get_height(inputs[0]);
    npixels = w * h;

    for (k = 0; k < npixels; k++) {

        win_offset = 0;

        if (mask != NULL && img_get_pixel(mask, k / mask->width, k % mask->width, 0) == 0) {
            continue;
        }

        for (i = 0; i < mop->levels[level].ninputs; i++) {
            offset_set(offset, mop->levels[level].windows[op][i], w, 1);
            for (j = 0; j < win_get_wsize(mop->levels[level].windows[op][i]); j++) {
                l = offset[j] + k;
                joint_wpat[win_offset] = img_get_pixel(inputs[i], l / w, l % w, 0);
                win_offset++;
            }
        }
        /* cria freq node */
        freq = freq_node_create(img_get_pixel(ideal, k / w, k % w, 0), 1);
        /* adiciona na arvore */
        xpl_GG_insert(xpl, (xpl_GG **) &xpl->root, joint_wpat, freq);
    }

    free(offset);
    free(joint_wpat);

    return xpl;
}

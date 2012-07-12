#include <trios_multi.h>

#include <stdlib.h>
#include <stdio.h>


xpl_t *collec_level_operator_bb_main(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask, img_t *ideal);

xpl_t *lcollec_multi_level(multi_level_operator_t *mop, int level, int op, img_t ***input, img_t **mask, img_t **ideal) {
    xpl_t *xpl = NULL, *xpl_new = NULL;
    int i;
    if (mop == NULL) {
        return trios_error(MSG, "Operator pointer is null");
    }
    if (level >= mop->nlevels) {
        return trios_error(MSG, "Operator has less than %d levels", level);
    }
    if (op >= mop->levels[level].noperators) {
        return trios_error(MSG, "Level %d contains less than %d operators.", op);
    }

    for (i = 0; i < mop->levels[level].ninputs; i++) {
        xpl_new = collec_level_operator_bb_main(mop, level, op, input[i], mask[i], ideal[i]);
        if (xpl == NULL) {
            xpl = xpl_new;
            xpl_new = NULL;
        } else if (xpl_new != NULL) {
            if (!xpl_merge(xpl_new, xpl)) {
                return trios_error(MSG, "multi-level collec: merge error");
            }
            xpl_free(xpl);
            xpl = xpl_new;
            xpl_new = NULL;
        }
    }

    return xpl;
}

xpl_t *collec_level_operator_bb_main(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask, img_t *ideal) {
    window_t *joint_win = NULL;
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
        for (i = 0; i < mop->levels[level].ninputs; i++) {
            offset_set(offset, multi_level_operator_get_window(mop, level, op, i), img_get_width(inputs[i]), 1);
            for (j = 0; j < win_get_wsize(mop->levels[level].windows[op][i]); j++) {
                int l = offset[j] + k;
                if (l >= 0 && l < npixels && inputs[i]->data[l] != 0) {
                    int index = (j + curr_win_size)/NB;
                    int bit = (j + curr_win_size) % NB;
                    joint_wpat[index] = (joint_wpat[index] | bitmsk[bit]);
                }
            }
            // seta os bits no joint_wpat
            curr_win_size += win_get_wsize(mop->levels[level].windows[op][i]);
        }
        // insere na árvore.
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

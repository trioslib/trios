#include <trios_multi.h>

#include <stdlib.h>
#include <stdio.h>

window_t *multi_level_operator_joint_window(int win_size, int i)
{
    window_t *joint_win = win_create(1, win_size, 1);
    for (i = 0; i < win_size; i++) {
        win_set_point(0, i,1, 1, joint_win);
    }
    return joint_win;
}

xpl_t *collec_level_operator_bb_main(multi_level_operator_t *mop, int level, int op, img_t **inputs) {
    window_t *joint_win = NULL;
    int i, j, k, l, npixels, win_size = 0;

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
        win_size += win_get_wsize(mop->levels[level].windows[op][i]);
    }
    xpl_t *xpl = xpl_create(win_size, BB);
    int *joint_wpat = malloc(sizeof(int) * size_of_zpat(win_size));
    int *offset = malloc(sizeof(int) * win_size);
    /* pega o tamanho das imagens */
    npixels = img_get_width(inputs[0]) * img_get_height(inputs[0]);
    for (k = 0; k < npixels; k++) {
        /* varre todo pixel */
        for (l = 0; l < win_size; l++) {
            joint_wpat[l] = 0;
        }
        int curr_win_size = 0;
        for (i = 0; i < mop->levels[level].ninputs; i++) {
            offset_set(offset, multi_level_operator_get_window(mop, level, op, i), img_get_width(inputs[i]), 1);
            // seta os bits no joint_wpat
            curr_win_size += win_get_wsize(mop->levels[level].windows[op][i]);
        }
        // insere na árvore.
    }
    return xpl;
}

#include <trios_multi.h>
#include <trios_error.h>
#include <trios_collec.h>
#include <trios.h>

#include <stdlib.h>

window_t *multi_level_operator_joint_window(multi_level_operator_t *mop, int level, int op)
{
    int win_size = 0, i;
    for (i = 0; i < mop->levels[level].ninputs; i++) {
        win_size += win_get_wsize(mop->levels[level].windows[op][i]);
    }
    window_t *joint_win = win_create(1, win_size, 1);
    for (i = 0; i < win_size; i++) {
        win_set_point(0, i,1, 1, joint_win);
    }
    return joint_win;
}



multi_level_operator_t *multi_level_build(multi_architecture_t *m, imgset_t *set) {
    int i, j, k;
    char filename[200];
    multi_level_operator_t *mop = multi_level_operator_create(m);

    img_t ***input_images;
    img_t ***new_input_images;
    img_t **mask_images;
    img_t **ideal_images;

    itv_t *starting_interval;

    trios_malloc(input_images, sizeof(img_t **) * imgset_get_ngroups(set), multi_level_operator_t *, "Bad alloc");
    for (i = 0; i < imgset_get_ngroups(set); i++) {
        trios_malloc(input_images[i], sizeof(img_t *), multi_level_operator_t *, "Bad alloc");
    }
    if (mop->nlevels > 1) {
        trios_malloc(new_input_images, sizeof(img_t **) * imgset_get_ngroups(set), multi_level_operator_t *, "Bad alloc");
    }
    trios_malloc(mask_images, sizeof(img_t *) * imgset_get_ngroups(set), multi_level_operator_t *, "Bad alloc");
    trios_malloc(ideal_images, sizeof(img_t *) * imgset_get_ngroups(set), multi_level_operator_t *, "Bad alloc");

    for (i = 0; i < imgset_get_ngroups(set); i++) {
        get_setofimages(set, BB, NULL, i+1, &(input_images[i][0]), ideal_images + i, mask_images + i);
    }

    for (i = 0; i < mop->nlevels; i++) {
        /*printf("Building level %d\n", i);*/
        if (i < mop->nlevels - 1) {
            for (j = 0; j < imgset_get_ngroups(set); j++) {
                trios_malloc(new_input_images[j], sizeof(img_t *) * mop->levels[i].noperators, multi_level_operator_t *, "Bad alloc");
            }
        }

        for (j = 0; j < mop->levels[i].noperators; j++) {
            /*printf("Operator %d ninputs %d \n", j, mop->levels[i].ninputs);*/
            /* faz collec em cada um dos operadores */
            xpl_t *op_collec = lcollec_multi_level(mop, i, j, input_images, mask_images, ideal_images, imgset_get_ngroups(set));
            /* decision em cada um dos operadores */
            mtm_t *op_dec = ldecision_memory(op_collec, 1, 0, AVERAGE, 0, 0);
            /* isi em cada um dos operadores = wait forever */
            window_t *joint_window = multi_level_operator_joint_window(mop, i, j);
            starting_interval = itv_gen_itv(joint_window, 1, BB, 0, 1, 0);
            itv_t *level_op = lisi_memory(op_dec, starting_interval, 3, 5, 0, 0);
            win_free(joint_window);
            xpl_free(op_collec);
            mop->levels[i].trained_operator[j] = level_op;
            /* if its the last level there is no need to apply the resulting operator. */
        }
        if (i < mop->nlevels -1) {
            /* muda os ponteiros dos input e ideal_images */
            for (j = 0; j < mop->levels[i].noperators; j++) {
                for(k = 0; k < imgset_get_ngroups(set); k++) {
                    /* aplica cada um dos operadores do nível e passa para o próximo nível treinar */
                    /*printf("APPLY %d, %d input %d\n\n", i, j, k);*/
                    sprintf(filename, "l%dop%d.pgm", i, j);
                    new_input_images[k][j] = multi_level_apply_level(mop, i, j, input_images[k]);
                    img_writePGM(filename, new_input_images[k][j]);
                }
            }
            for(k = 0; k < imgset_get_ngroups(set); k++) {
                for (j = 0; j < mop->levels[i].ninputs; j++) {
                    img_free(input_images[k][j]);
                }
                free(input_images[k]);
            }
            free(input_images);
            input_images = new_input_images;
        }
    }
    for(k = 0; k < imgset_get_ngroups(set); k++) {
        img_free(mask_images[k]);
        img_free(ideal_images[k]);
        for (j = 0; j < mop->levels[mop->nlevels-1].ninputs; j++) {
            img_free(input_images[k][j]);
        }
        free(input_images[k]);
    }
    free(mask_images);
    free(ideal_images);
    free(input_images);
    return mop;
}

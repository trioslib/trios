#include <trios_multi.h>
#include <trios_error.h>
#include <trios_collec.h>
#include <trios.h>

#include "../collec/collec_private.h"

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



static int load_image_set(img_t ****__input_images, img_t ***__ideal_images, img_t ***__mask_images, imgset_t **set, int n, multi_level_operator_t *mop) {
    int i;
    img_t ***input_images, **mask_images, **ideal_images;
    trios_malloc(input_images, sizeof(img_t **) * imgset_get_ngroups(set[n]), int, "Bad alloc");
    for (i = 0; i < imgset_get_ngroups(set[n]); i++) {
        trios_malloc(input_images[i], sizeof(img_t *), int, "Bad alloc");
    }
    /*if (mop->nlevels > 1) {
        trios_malloc(new_input_images, sizeof(img_t **) * imgset_get_ngroups(set[0]), multi_level_operator_t *, "Bad alloc");
    }*/
    trios_malloc(mask_images, sizeof(img_t *) * imgset_get_ngroups(set[n]), int, "Bad alloc");
    trios_malloc(ideal_images, sizeof(img_t *) * imgset_get_ngroups(set[n]), int, "Bad alloc");

    for (i = 0; i < imgset_get_ngroups(set[n]); i++) {
        get_setofimages(set[n], BB, NULL, i+1, &(input_images[i][0]), ideal_images + i, mask_images + i);
    }
    *__input_images = input_images;
    *__mask_images = mask_images;
    *__ideal_images = ideal_images;
}

static int build_level(multi_level_operator_t *mop, int level, imgset_t **set, int set_idx, img_t ***input_images, img_t **ideal_images, img_t **mask_images) {
    int j;
    itv_t *starting_interval;

    for (j = 0; j < mop->levels[level].noperators; j++) {
        /*printf("Operator %d ninputs %d \n", j, mop->levels[i].ninputs);*/
        /* faz collec em cada um dos operadores */
        xpl_t *op_collec = lcollec_multi_level(mop, level, j, input_images, mask_images, ideal_images, imgset_get_ngroups(set[set_idx]));
        /* decision em cada um dos operadores */
        mtm_t *op_dec = ldecision_memory(op_collec, 1, 0, AVERAGE, 0, 0);
        /* isi em cada um dos operadores = wait forever */
        window_t *joint_window = multi_level_operator_joint_window(mop, level, j);
        starting_interval = itv_gen_itv(joint_window, 1, BB, 0, 1, 0);
        itv_t *level_op = lisi_memory(op_dec, starting_interval, 3, 5, 0, 0);
        win_free(joint_window);
        xpl_free(op_collec);
        mop->levels[level].trained_operator[j] = level_op;
    }
}

static int free_images(img_t ***input_images, img_t **ideal_images, img_t **mask_images, imgset_t **set, int set_idx, multi_level_operator_t *mop, int level, int free_all) {
    int j, k;
    for(k = 0; k < imgset_get_ngroups(set[set_idx]); k++) {
        if (free_all) {
            img_free(mask_images[k]);
            img_free(ideal_images[k]);
        }
        for (j = 0; j < mop->levels[level].ninputs; j++) {
            img_free(input_images[k][j]);
        }
        free(input_images[k]);
    }
}

multi_level_operator_t *multi_level_build_single(multi_architecture_t *m, imgset_t *set) {
    int i, j, k;
    char filename[200];
    multi_level_operator_t *mop = multi_level_operator_create(m);

    img_t ***input_images;
    img_t ***new_input_images;
    img_t **mask_images;
    img_t **ideal_images;

    itv_t *starting_interval;

    if (mop->nlevels > 1) {
        trios_malloc(new_input_images, sizeof(img_t **) * imgset_get_ngroups(set), multi_level_operator_t *, "Bad alloc");
    }
    load_image_set(&input_images, &ideal_images, &mask_images, &set, 0, mop);

    for (i = 0; i < mop->nlevels; i++) {
        /*printf("Building level %d\n", i);*/
        if (i < mop->nlevels - 1) {
            for (j = 0; j < imgset_get_ngroups(set); j++) {
                trios_malloc(new_input_images[j], sizeof(img_t *) * mop->levels[i].noperators, multi_level_operator_t *, "Bad alloc");
            }
        }
        build_level(mop, i, &set, 0, input_images, ideal_images, mask_images);

        if (i < mop->nlevels -1) {
            /* muda os ponteiros dos input e ideal_images */
            for (j = 0; j < mop->levels[i].noperators; j++) {
                for(k = 0; k < imgset_get_ngroups(set); k++) {
                    sprintf(filename, "l%dop%d.pgm", i, j);
                    new_input_images[k][j] = multi_level_apply_level(mop, i, j, input_images[k]);
                    img_writePGM(filename, new_input_images[k][j]);
                }
            }
            free_images(input_images, ideal_images, mask_images, &set, 0, mop, i, 0);
            input_images = new_input_images;
        }
    }
    free_images(input_images, ideal_images, mask_images, &set, 0, mop, mop->nlevels - 1, 1);
    free(mask_images);
    free(ideal_images);
    free(input_images);
    return mop;
}

static int apply_until_level_images(multi_level_operator_t *mop, int level, img_t *img, img_t ***__next) {
    img_t **input, **next;
    char cmd[1024];
    int i, j, k;
    input = &img;
    trios_malloc(next, sizeof(img_t *) * mop->levels[0].noperators, int, "Bad alloc");
    for (i = 0; i <= level; i++) {
        for (j = 0; j < mop->levels[i].noperators; j++) {
            next[j] = multi_level_apply_level(mop, i, j, input);
            sprintf(cmd, "l%dop%d.pgm", i, j);
            img_writePGM(cmd, next[j]);
        }
        if (i > 0) {
            for (j = 0; j < mop->levels[i].ninputs; j++) {
                img_free(input[j]);
            }
            free(input);
        }
        input = next;
        if (i < level) {
            trios_malloc(next, sizeof(img_t *) * mop->levels[i].noperators, int, "Bad alloc");
        }
    }
    /*free(input);*/
    *__next = next;
    return 1;
}


multi_level_operator_t *multi_level_build(multi_architecture_t *m, imgset_t **set) {
    int i, j, k;
    multi_level_operator_t *mop = multi_level_operator_create(m);

    img_t ***input_images;
    img_t ***new_input_images;
    img_t **mask_images;
    img_t **ideal_images;

    load_image_set(&input_images, &ideal_images, &mask_images, set, 0, mop);

    for (i = 0; i < mop->nlevels; i++) {
        printf("Operator %d\n", i);
        build_level(mop, i, set, i, input_images, ideal_images, mask_images);
        if (i < mop->nlevels - 1) {
            /* dou free em tudo do nivel i */
            free_images(input_images, ideal_images, mask_images, set, i, mop, i, 1);
            /* carrego as imagens do nivel i + 1 */
            load_image_set(&input_images, &ideal_images, &mask_images, set, i+1, mop);
            /* create vector to store the transformed input images for level i+1 */
            trios_malloc(new_input_images, sizeof(img_t **) * imgset_get_ngroups(set[i+1]), multi_level_operator_t *, "Bad alloc");
            for (j = 0; j < imgset_get_ngroups(set[i+1]); j++) {
                trios_malloc(new_input_images[j], sizeof(img_t *) * mop->levels[i].noperators, multi_level_operator_t *, "Bad alloc");
            }
            /* aplico o operador até o nível i */
            for (j = 0; j < imgset_get_ngroups(set[i+1]); j++) {
                apply_until_level_images(mop, i, input_images[j][0], &(new_input_images[j]));
            }
            /* coloca o ponteiro input_images como o resultado da aplicação acima */
            input_images = new_input_images;

        }

    }

    free(mask_images);
    free(ideal_images);
    free(input_images);
    return mop;
}

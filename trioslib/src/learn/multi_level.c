#include <trios_multi.h>
#include <trios_error.h>
#include <trios_collec.h>
#include <trios.h>

#include "../collec/collec_private.h"
#include "../appl/local.h"

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

window_t *biggest_window(multi_architecture_t *m) {
    window_t *big = NULL;
    int max_h = -1, max_w = -1;
    int i, j, k;
    for (i = 0; i < m->nlevels; i++) {
        for (j = 0; j < m->levels[i].noperators; j++) {
            for (k = 0; k < m->levels[i].ninputs; k++) {
                if (win_get_width(m->levels[i].windows[j][k]) > max_w) {
                    max_w = win_get_width(m->levels[i].windows[j][k]);
                }
                if (win_get_height(m->levels[i].windows[j][k]) > max_h) {
                    max_h = win_get_height(m->levels[i].windows[j][k]);
                }
            }
        }
    }
    big = win_create(max_h, max_w, 1);
    return big;
}

static int load_image_set(img_t ****__input_images, img_t ***__ideal_images, img_t ***__mask_images, imgset_t **set, int n, multi_level_operator_t *mop, multi_architecture_t *march) {
    int i;
    img_t ***input_images, **mask_images, **ideal_images;
    window_t *big;
    trios_malloc(input_images, sizeof(img_t **) * imgset_get_ngroups(set[n]), int, "Bad alloc");
    for (i = 0; i < imgset_get_ngroups(set[n]); i++) {
        trios_malloc(input_images[i], sizeof(img_t *), int, "Bad alloc");
    }
    /*if (mop->nlevels > 1) {
        trios_malloc(new_input_images, sizeof(img_t **) * imgset_get_ngroups(set[0]), multi_level_operator_t *, "Bad alloc");
    }*/
    trios_malloc(mask_images, sizeof(img_t *) * imgset_get_ngroups(set[n]), int, "Bad alloc");
    trios_malloc(ideal_images, sizeof(img_t *) * imgset_get_ngroups(set[n]), int, "Bad alloc");

    big = biggest_window(march);
    for (i = 0; i < imgset_get_ngroups(set[n]); i++) {
        get_setofimages(set[n], BB, big, i+1, &(input_images[i][0]), ideal_images + i, mask_images + i);
    }
    *__input_images = input_images;
    *__mask_images = mask_images;
    *__ideal_images = ideal_images;
}

static int build_level(multi_level_operator_t *mop, int level, imgset_t **set, int set_idx, img_t ***input_images, img_t **ideal_images, img_t **mask_images) {
    int j;
    classifier_t *level_op = NULL;
    xpl_t *op_collec;
    mtm_t *op_dec;
    window_t *joint_window;
    char temp[1024];


    for (j = 0; j < mop->levels[level].noperators; j++) {
        /*printf("Operator %d ninputs %d \n", j, mop->levels[i].ninputs);*/
        /* faz collec em cada um dos operadores */
        joint_window = multi_level_operator_joint_window(mop, level, j);
        op_collec = lcollec_multi_level(mop, level, j, input_images, mask_images, ideal_images, imgset_get_ngroups(set[set_idx]));
        /* decision em cada um dos operadores */
        op_dec = ldecision_memory(op_collec, mop->type == BB, 0, MEDIAN, 0, 0);
#define DEBUG
#ifdef DEBUG
        if (mop->type == GG) {
            sprintf(temp, "GG-lv%d-op%d.xp", level, j);
            xpl_write(temp, op_collec, joint_window, NULL);
            sprintf(temp, "GG-lv%d-op%d.mtm", level, j);
            mtm_write(temp, op_dec, joint_window, NULL);
        } else {
            sprintf(temp, "BB-lv%d-op%d.xp", level, j);
            xpl_write(temp, op_collec, joint_window, NULL);
            sprintf(temp, "BB-lv%d-op%d.mtm", level, j);
            mtm_write(temp, op_dec, joint_window, NULL);
        }
#endif
#undef DEBUG
        if (level == 0) {
            if (mop->type == BB) {
                level_op = (classifier_t *) lisi_partitioned(mop->levels[0].windows[j][0], op_dec, 13000);
            } else if (mop->type == GG) {
                level_op = (classifier_t *) ltrainGG_memory(op_dec);
            }
        } else {
            if (mop->type == BB) {
                level_op = (classifier_t *) lisi_partitioned(joint_window, op_dec, 30000);
            } else if (mop->type == GG) {
                level_op = (classifier_t *) ltrainGG_memory(op_dec);
            }
            win_free(joint_window);
        }
        xpl_free(op_collec);
        mtm_free(op_dec);
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
    multi_level_operator_t *mop = multi_level_operator_create(m, BB);

    img_t ***input_images;
    img_t ***new_input_images;
    img_t **mask_images;
    img_t **ideal_images;

    /*itv_t *starting_interval;*/
    itv_t *level_op;
    window_t *big;

    if (mop->nlevels > 1) {
        trios_malloc(new_input_images, sizeof(img_t **) * imgset_get_ngroups(set), multi_level_operator_t *, "Bad alloc");
    }

    load_image_set(&input_images, &ideal_images, &mask_images, &set, 0, mop, m);

    for (i = 0; i < mop->nlevels; i++) {
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
                    new_input_images[k][j] = multi_level_apply_level_bb(mop, i, j, input_images[k], mask_images[k]);
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

static int apply_until_level_images(multi_level_operator_t *mop, int level, img_t *img, img_t *mask, img_t ***__next) {
    img_t **input, **next;
    char cmd[1024];
    int i, j, k;
    input = &img;
    trios_malloc(next, sizeof(img_t *) * mop->levels[level].noperators, int, "Bad alloc");
    for (i = 0; i <= level; i++) {

#pragma omp parallel
{ 
	#pragma omp for schedule(static, 1) 
        for (j = 0; j < mop->levels[i].noperators; j++) {
            printf("Thread %d, max %d\n", j, mop->levels[i].noperators);
            if (mop->type == BB) {
                next[j] = multi_level_apply_level_bb(mop, i, j, input, mask);
            } else if (mop->type == GG) {
                next[j] = multi_level_apply_level_gg(mop, i, j, input, mask);
            }
            /*sprintf(cmd, "l%dop%d.pgm", i, j);
            img_writePGM(cmd, next[j]);*/
        }
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

multi_level_operator_t *multi_level_combine_operators(image_operator_t **ops, int nops, imgset_t *level2) {
    multi_architecture_t *arch;
    multi_level_operator_t *mop;
    window_t *two_level;
    img_t ***input_images;
    img_t ***lv2_images;
    img_t **mask_images;
    img_t **ideal_images;
    int i, j;
    int levels[] = {0, 1};
    levels[0] = nops;
    arch = multi_level_arch_create(2, levels);
    two_level = win_create(1, 1, 1);
    win_set_point(0, 0, 1, 1, two_level);
    for (i = 0; i < nops; i++) {
        multi_level_arch_set_window(arch, 0, i, 0, ops[i]->win);
        multi_level_arch_set_window(arch, 1, 0, i, two_level);
    }
    mop = multi_level_operator_create(arch, ops[0]->type);
    for (i = 0; i < nops; i++) {
        if (mop->type == BB) {
            mop->levels[0].trained_operator[i] = (classifier_t *) ops[i]->bb;
        } else {
            mop->levels[0].trained_operator[i] = (classifier_t *) ops[i]->gg;
        }
    }

    /* carrega imagens */

    load_image_set(&input_images, &ideal_images, &mask_images, &level2, 0, mop, arch);

    trios_malloc(lv2_images, sizeof(img_t **) * imgset_get_ngroups(level2), multi_level_operator_t *, "Bad alloc");
    for (j = 0; j < imgset_get_ngroups(level2); j++) {
        trios_malloc(lv2_images[j], sizeof(img_t *) * mop->levels[0].noperators, multi_level_operator_t *, "Bad alloc");
    }

    for (j = 0; j < imgset_get_ngroups(level2); j++) {
        apply_until_level_images(mop, 0, input_images[j][0], mask_images[j], &(lv2_images[j]));
    }
    build_level(mop, 1, &level2, 0, lv2_images, ideal_images, mask_images);

    free_images(input_images, ideal_images, mask_images, &level2, 0, mop, 0, 1);
    for (j = 0; j < imgset_get_ngroups(level2); j++) {
        for (i = 0; i < nops; i++) {
            img_free(lv2_images[j][i]);
        }
        free(lv2_images[j]);
    }
    free(lv2_images);
    multi_level_arch_free(arch);
    return mop;
}

multi_level_operator_t *multi_level_build_bb(multi_architecture_t *m, imgset_t **set) {
    return multi_level_build(m, set, BB);
}

multi_level_operator_t *multi_level_build_gg(multi_architecture_t *m, imgset_t **set) {
    return multi_level_build(m, set, GG);
}

multi_level_operator_t *multi_level_build(multi_architecture_t *m, imgset_t **set, int type) {
    int i, j, k;
    multi_level_operator_t *mop = multi_level_operator_create(m, type);

    img_t ***input_images;
    img_t ***new_input_images;
    img_t **mask_images;
    img_t **ideal_images;

    load_image_set(&input_images, &ideal_images, &mask_images, set, 0, mop, m);

    for (i = 0; i < mop->nlevels; i++) {
        printf("Operator %d\n", i);
        build_level(mop, i, set, i, input_images, ideal_images, mask_images);
        if (i < mop->nlevels - 1) {
            /* dou free em tudo do nivel i */
            free_images(input_images, ideal_images, mask_images, set, i, mop, i, 1);
            /* carrego as imagens do nivel i + 1 */
            load_image_set(&input_images, &ideal_images, &mask_images, set, i+1, mop, m);
            /* create vector to store the transformed input images for level i+1 */
            trios_malloc(new_input_images, sizeof(img_t **) * imgset_get_ngroups(set[i+1]), multi_level_operator_t *, "Bad alloc");
            for (j = 0; j < imgset_get_ngroups(set[i+1]); j++) {
                trios_malloc(new_input_images[j], sizeof(img_t *) * mop->levels[i].noperators, multi_level_operator_t *, "Bad alloc");
            }
            /* aplico o operador até o nível i */
            for (j = 0; j < imgset_get_ngroups(set[i+1]); j++) {
                apply_until_level_images(mop, i, input_images[j][0], mask_images[j], &(new_input_images[j]));
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

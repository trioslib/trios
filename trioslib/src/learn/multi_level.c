#include <trios_multi.h>
#include <trios_error.h>
#include <trios_collec.h>
#include <trios.h>

#include <stdlib.h>

multi_level_operator_t *multi_level_build(multi_architecture_t *m, imgset_t *set) {
    int i, j, k;
    multi_level_operator_t *mop = multi_level_operator_create(m);

    img_t ***input_images;
    img_t ***new_input_images;
    img_t **mask_images;
    img_t **ideal_images;

    trios_malloc(input_images, sizeof(img_t **) * imgset_get_ngroups(set), "Bad alloc");
    trios_malloc(new_input_images, sizeof(img_t **) * imgset_get_ngroups(set), "Bad alloc");
    for (i = 0; i < imgset_get_ngroups(set); i++) {
        trios_malloc(input_images[i], sizeof(img_t *), "Bad alloc");
    }
    trios_malloc(mask_images, sizeof(img_t *) * imgset_get_ngroups(set), "Bad alloc");
    trios_malloc(ideal_images, sizeof(img_t *) * imgset_get_ngroups(set), "Bad alloc");

    for (i = 0; i < imgset_get_ngroups(set); i++) {
        get_setofimages(set, BB, NULL, i+1, &(input_images[i][0]), ideal_images + i, mask_images + i);
    }

    for (i = 0; i < mop->nlevels; i++) {
        if (i < mop->nlevels - 1) {
            for (j = 0; j < imgset_get_ngroups(set); j++) {
                trios_malloc(new_input_images[i], sizeof(img_t *) * mop->levels[i+1].ninputs, "Bad alloc");
            }
        }

        for (j = 0; j < mop->levels[i].noperators; j++) {
            /* faz collec em cada um dos operadores */
            xpl_t *op_collec = lcollec_multi_level(mop, i, j, input_images, mask_images, ideal_images);
            /* decision em cada um dos operadores */
            //mtm_t *dec = ldecision();
            /* isi em cada um dos operadores = wait forever */
            //itv_t *level_op = lisi(....)
            /* aplica cada um dos operadores do nível e passa para o próximo nível treinar */

            /* muda os ponteiros dos input e ideal_images */
            if (i < mop->nlevels - 1) {

            }
        }

    }
    /* free everything */
    return mop;
}

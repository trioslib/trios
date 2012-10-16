#include "trios.h"
#include "../collec/collec_private.h"

int computeMAEBB(itv_t *bb_operator, window_t *win, imgset_t *test, double *acc) {
    int i, j, k;
    int n_images;
    int MAE = 0, n_pixels;
    img_t *input, *ideal, *mask, *result;
    char *filename;

    n_images = imgset_get_ngroups(test);
    n_pixels = 0;
    for (k = 0; k < n_images; k++) {
        get_setofimages(test, BB, win, k+1, &input, &ideal, &mask);
        result = lpapplic_memory(input, bb_operator, win, mask, 0, 0, 255);
        /* compara result com ideal */
        for (i = 0; i < img_get_height(input); i++) {
            for (j = 0; j < img_get_width(input); j++) {
                if (img_get_pixel(result, i, j, 0) != img_get_pixel(ideal, i, j, 0)) {
                    MAE++;
                }
            }
        }
        n_pixels += img_get_height(input) * img_get_width(input);
        img_free(input);
        img_free(ideal);
        img_free(mask);
        img_free(result);
    }
    if (acc != NULL) {
        *acc = 1.0 * MAE / n_pixels;
    }

    return MAE;
}

int computeMAEBBmulti(multi_level_operator_t *bb_operator, imgset_t *test, double *acc) {
    int i, j, k;
    int n_images;
    int MAE = 0, n_pixels;
    img_t *input, *ideal, *mask, *result;
    char *filename;

    n_images = imgset_get_ngroups(test);
    n_pixels = 0;
    for (k = 0; k < n_images; k++) {
        get_setofimages(test, BB, NULL, k+1, &input, &ideal, &mask);
        result = multi_level_apply(bb_operator, input);
        /* compara result com ideal */
        for (i = 0; i < img_get_height(input); i++) {
            for (j = 0; j < img_get_width(input); j++) {
                if (img_get_pixel(result, i, j, 0) != img_get_pixel(ideal, i, j, 0)) {
                    MAE++;
                }
            }
        }
        n_pixels += img_get_height(input) * img_get_width(input);
        img_free(input);
        img_free(ideal);
        img_free(mask);
        img_free(result);
    }
    if (acc != NULL) {
        *acc = 1.0 * MAE / n_pixels;
    }

    return MAE;
}

int computeMSEGG(dTree *gg_operator, window_t *win, imgset_t *test, double *acc) {
    int i, j, k;
    int n_images;
    unsigned long long MSE = 0, n_pixels, wrong = 0;
    img_t *input, *ideal, *mask, *result;
    char *filename;

    n_images = imgset_get_ngroups(test);
    n_pixels = 0;
    for (k = 0; k < n_images; k++) {
        get_setofimages(test, GG, win, k+1, &input, &ideal, &mask);
        result = lapplyGG_memory(input, gg_operator, win);
        img_writePGM("SDF.pgm", result);
        /* compara result com ideal */
        for (i = 0; i < img_get_height(input); i++) {
            for (j = 0; j < img_get_width(input); j++) {
                MSE += pow(img_get_pixel(result, i, j, 0) - img_get_pixel(ideal, i, j, 0), 2);
                if (img_get_pixel(result, i, j, 0) != img_get_pixel(ideal, i, j, 0)) {
                    wrong++;
                }
            }
        }
        n_pixels += img_get_height(input) * img_get_width(input);
        img_free(input);
        img_free(ideal);
        img_free(mask);
        img_free(result);
    }
    *acc = (double) wrong / n_pixels;
    return MSE;
}

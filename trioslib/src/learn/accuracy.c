#include "trios.h"
#include "../collec/collec_private.h"
#include <stdio.h>
#include <time.h>

#define DEBUG

unsigned long image_operator_mae(image_operator_t *iop, imgset_t *test, double *acc) {
    int i, j, k;
    int n_images;
    unsigned long total_MAE = 0, *mae_images, *n_pixels, pixels_total;

    img_t *input, *ideal, *mask, *result;

    n_images = imgset_get_ngroups(test);
    pixels_total = 0;

    trios_malloc(mae_images, sizeof(unsigned long) * n_images, unsigned long, "Error allocating mae_images");
    trios_malloc(n_pixels, sizeof(unsigned long) * n_images, unsigned long, "Error allocating n_pixels");

#pragma omp parallel for shared(mae_images, n_pixels) private(k, input, ideal, mask, result, i, j)
    for (k = 0; k < n_images; k++) {
        if (iop->type == BB) {
            get_setofimages(test, BB, iop->win, k+1, &input, &ideal, &mask);
        } else {
            get_setofimages(test, GG, iop->win, k+1, &input, &ideal, &mask);
        }
        result = image_operator_apply(iop, input, mask);
        mae_images[k] = n_pixels[k] = 0;
        for (i = 0; i < img_get_height(input); i++) {
            for (j = 0; j < img_get_width(input); j++) {
                if (img_get_pixel(mask, i, j, 0) != 0) {
                    n_pixels[k]++;
                    if (img_get_pixel(result, i, j, 0) != img_get_pixel(ideal, i, j, 0)) {
                        mae_images[k]++;
                    }
                }
            }
        }

        img_free(input);
        img_free(ideal);
        img_free(mask);
        img_free(result);
    }

    /* sums all mae_images into MAE and n_pixels into pixels_total */
    total_MAE = pixels_total = 0;
    for (k = 0; k < n_images; k++) {
        total_MAE += mae_images[k];
        pixels_total += n_pixels[k];
    }

    if (acc != NULL) {
        *acc = 1.0 * total_MAE / pixels_total;
    }

    return total_MAE;
}


unsigned long image_operator_mse(image_operator_t *iop, imgset_t *test) {
    int i, j, k, l, m;
    int n_images;
    unsigned long long total_MSE = 0, *mse_images;
    img_t *input, *ideal, *mask, *result;

    n_images = imgset_get_ngroups(test);
    trios_malloc(mse_images, sizeof(unsigned long long) * n_images, unsigned long, "Error allocating mse_images");

#pragma omp parallel for shared(mse_images) private(k, input, ideal, mask, result, i, j)
    for (k = 0; k < n_images; k++) {
        if (iop->type == BB) {
            get_setofimages(test, BB, iop->win, k+1, &input, &ideal, &mask);
        } else {
            get_setofimages(test, GG, iop->win, k+1, &input, &ideal, &mask);
        }
        result = image_operator_apply(iop, input, mask);
        mse_images[k] = 0;
        for (i = 0; i < img_get_height(input); i++) {
            for (j = 0; j < img_get_width(input); j++) {
                mse_images[k] += pow(img_get_pixel(result, i, j, 0) - img_get_pixel(ideal, i, j, 0), 2);
            }
        }

        img_free(input);
        img_free(ideal);
        img_free(mask);
        img_free(result);
    }

    total_MSE = 0;
    for (k = 0; k < n_images; k++) {
        total_MSE += mse_images[k];
    }

    return total_MSE;
}

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
        img_writePGM("mask_s.pgm", mask);
        result = lpapplic_memory(input, bb_operator, win, mask, 0, 0, 255);
        /* compara result com ideal */
        for (i = 0; i < img_get_height(input); i++) {
            for (j = 0; j < img_get_width(input); j++) {
                if (img_get_pixel(mask, i, j, 0) != 0) {
                    n_pixels++;
                    if (img_get_pixel(result, i, j, 0) != img_get_pixel(ideal, i, j, 0)) {
                        MAE++;
                    }
                }
            }
        }
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
        get_setofimages(test, BB, bb_operator->levels[0].windows[0][0], k+1, &input, &ideal, &mask);
        img_writePGM("mask_m.pgm", mask);
        result = multi_level_apply(bb_operator, input, mask);
        /* compara result com ideal */
        for (i = 0; i < img_get_height(input); i++) {
            for (j = 0; j < img_get_width(input); j++) {
                if (img_get_pixel(mask, i, j, 0) != 0) {
                    n_pixels++;
                    if (img_get_pixel(result, i, j, 0) != img_get_pixel(ideal, i, j, 0)) {
                        MAE++;
                    }
                }

            }
        }
        /*n_pixels += (img_get_width(input) - win_get_width(bb_operator->levels[0].windows[0][0]) - 1) *
                    (img_get_height(input) - win_get_height(bb_operator->levels[0].windows[0][0]) - 1);*/
        img_free(input);
        img_free(ideal);
        img_free(mask);
        img_free(result);
    }
    printf("Test pixels %d\n", n_pixels);
    if (acc != NULL) {
        *acc = 1.0 * MAE / n_pixels;
    }

    return MAE;
}

int computeMSEGG(dTree *gg_operator, window_t *win, imgset_t *test, double *acc) {
    int i, j, k, l, m;
    int n_images;
    unsigned long long MSE = 0, n_pixels, wrong = 0;
    img_t *input, *ideal, *mask, *result;

    n_images = imgset_get_ngroups(test);
    n_pixels = 0;
    for (k = 0; k < n_images; k++) {
        get_setofimages(test, GG, win, k+1, &input, &ideal, &mask);
        result = lapplyGG_memory(input, gg_operator, win, mask);

        /* compara result com ideal */
        for (i = 0; i < img_get_height(input); i++) {
            for (j = 0; j < img_get_width(input); j++) {
                /*MSE += pow(img_get_pixel(result, i, j, 0) - img_get_pixel(ideal, i, j, 0), 2);*/
                /*MSE += (img_get_pixel(result, i, j, 0) == img_get_pixel(ideal, i, j, 0));*/
                if (img_get_pixel(mask, i, j, 0) != 0) {
                    n_pixels++;
                    if (img_get_pixel(result, i, j, 0) != img_get_pixel(ideal, i, j, 0)) {
                        wrong++;
                        MSE += pow(img_get_pixel(result, i, j, 0) - img_get_pixel(ideal, i, j, 0), 2);
                    }
                }
            }
        }
        img_free(input);
        img_free(ideal);
        img_free(mask);
        img_free(result);
    }
    *acc = (double) wrong / n_pixels;
    return MSE;
}

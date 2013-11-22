#include "trios.h"


xpl_t *lcollec_two_level(imgset_t *imgset, image_operator_t **ops, int nops) {
    img_t *input, *output, *mask, **results;
    int i, j, k, n_pixels, n_images, w, h, r, sht, bit;
    unsigned int *wpat;
    window_t *one;
    xpl_t *xpl_result;

    n_images = imgset_get_ngroups(imgset);
    wpat = malloc(sizeof(int) * (nops + 31)/32);
    results = malloc(sizeof(img_t *) * nops);
    one = win_create(1, 1, 1);
    win_set_point(0, 0, 1, 1, one);

    xpl_result = xpl_create(nops, ops[0]->type);

    for (k = 0; k < n_images; k++) {
        fprintf(stderr, "Image %d\n", k);
        get_setofimages(imgset, ops[0]->type, one, k+1, &input, &output, &mask);

        #pragma omp parallel for
        for (j = 0; j < nops; j++) {
            results[j] = image_operator_apply(ops[j], input, mask);
        }
        w = img_get_width(results[0]);
        h = img_get_height(results[0]);
        n_pixels = w * h;
        for (i = 0; i < n_pixels; i++) {
            if (img_get_pixel(mask, i/w, i%w, 0) == 0) continue;

            for (j = 0; j < (nops + 31)/32; j++) {
                wpat[j] = 0;
            }
            for (j = 0; j < nops; j++) {
                r = img_get_pixel(results[j], i / w, i % w, 0);
                if (r != 0) {
                    wpat[j/NB] = wpat[j/NB] | (1 << (j%NB));
                }
            }
            r = img_get_pixel(output, i / w, i % w, 0);
            xpl_BB_insert(xpl_result, (xpl_BB **) &xpl_result->root, wpat, r == 0, r != 0);
        }

        for (j = 0; j < nops; j++) {
            img_free(results[j]);
        }
        img_free(input);
        img_free(output);
        img_free(mask);
    }

    return xpl_result;
}

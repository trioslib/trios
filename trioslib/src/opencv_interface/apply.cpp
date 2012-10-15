
#include "opencv2/ml/ml.hpp"
#include "opencv2/opencv.hpp"

#include "trios_img.h"
#include "trios_apert.h"
#include "trios_mtm.h"
#include "trios_win.h"

#include "trios_misc.h"
#include "trios_common.h"

#include "stdio.h"

typedef void dTree;

img_t *apply(img_t *img, CvDTree *tr, window_t *win) {
    img_t *output = img_create(img->width, img->height, img->nbands, img->pixel_size);
    cv::Mat wpat(1, win->wsize, CV_32FC1);
    int *offset = offset_create(win_get_wsize(win));
    offset_set(offset, win, img->width, 1);

    int n = img->width * img->height;
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < win_get_wsize(win); j++) wpat.at<float>(0, j) = 0.0;
        /* monta padrao */
        for (int j = 0; j < win_get_wsize(win); j++) {
            int l = k + offset[j];
            if (l >= 0 && l < n) {
                wpat.at<float>(0, j) = (unsigned char) img_get_pixel(img, l / img->width, l % img->width, 0);
            }
        }
        /* classifica */
        CvDTreeNode *node= tr->predict(wpat);
        unsigned int val = (unsigned int) (node->value + 0.5);
        /* coloca na imagem */
        img_set_pixel(output, k / img->width, k % img->width, 0, val);
    }
    return output;
}

img_t *applyWK(img_t *img, CvDTree *tr, window_t *win, apert_t *apt) {
    img_t *output = img_create(img->width, img->height, img->nbands, img->pixel_size);
    cv::Mat wpat(1, win->wsize, CV_32FC1);
    int *offset = offset_create(win_get_wsize(win));
    offset_set(offset, win, img->width, 1);

    int ki = apt->ki[0];
    int ko = apt->ko[0];
    int vplace = apt->vplace[0];

    int n = img->width * img->height;
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < win_get_wsize(win); j++) wpat.at<float>(0, j) = 0.0;
        /* monta padrao */
        for (int j = 0; j < win_get_wsize(win); j++) {
            int l = k + offset[j];
            if (l >= 0 && l < n) {
                float f = (float) img_get_pixel(img, l / img->width, l % img->width, 0);
                if (ki != 0) {
                    if (vplace == VP_Center) {
                        f -= img_get_pixel(img, k / img->width, k % img->width, 0);
                    } else if (vplace == VP_Median) {

                    }
                    if (f > ki) f = ki;
                    if (f < -ki) f = -ki;
                }
                wpat.at<float>(0, j) = f;
            }
        }
        /* classifica */
        CvDTreeNode *node= tr->predict(wpat);
        int val = node->value;
        if (apt->ko != 0) {
            if (vplace == VP_Center) {
                val += img_get_pixel(img, k / img->width, k % img->width, 0);
            }
        }
        /* coloca na imagem */
        img_set_pixel(output, k / img->width, k % img->width, 0, (unsigned char) val);
    }
    return output;
}


extern "C" img_t *apply_cv_tree(img_t *input, dTree *tree, window_t *win) {
    return apply(input, (CvDTree *)tree, win);
}

extern "C" img_t *apply_cv_treeWK(img_t *input, dTree *tree, window_t *win, apert_t *apt) {
    return applyWK(input, (CvDTree *)tree, win, apt);
}

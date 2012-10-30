#include <cstdio>

#include "opencv2/opencv.hpp"
#include "opencv2/ml/ml.hpp"
#include "trios_mtm.h"
#include "trios_io.h"

cv::Mat build_classes_from_mtm(mtm_t *mtm) {
    int width = 1;
    int height = mtm->nsum;
    cv::Mat classes(height, width, CV_32FC1);
    int k = 0;
    for (int i = 0; i < mtm->nmtm; i++) {
        mtm_GX *m = (mtm_GX *) mtm->mtm_data + i;
        for (int j = 0; j < m->fq; j++, k++) {
            classes.at<float>(k, 0) = m->label;
        }
    }
    return classes;
}

cv::Mat build_samples_from_mtm(mtm_t *mtm) {
    int width = mtm->wsize;
    int height = mtm->nsum;
    cv::Mat samples(height, width, CV_32FC1);

    for (int i = 0, k = 0; i < mtm->nmtm; i++) {
        mtm_GX *m = (mtm_GX *) mtm->mtm_data + i;
        for (int j = 0; j < m->fq; j++, k++) {
            for (int l = 0; l < mtm->wsize; l++) {
                samples.at<float>(k, l) = (unsigned char) m->wpat[l];
            }
        }
    }
    return samples;
}

void test_accuracy(CvDTree &tr, cv::Mat samples, cv::Mat labels) {
    cv::Mat wpat(1, samples.cols, CV_32FC1);
    unsigned long right, wrong, mse;
    float value;
    right = wrong = mse = 0;
    for (int i = 0; i < samples.rows; i++) {
        for (int j = 0; j < samples.cols; j++) {
            wpat.at<float>(0, j) = samples.at<float>(i, j);
        }

        CvDTreeNode *node = tr.predict(wpat);
        //value = tr.predict(wpat);
        value = node->value;
        if (value == labels.at<float>(i, 0)) {
            right++;
        } else {
            mse += pow(value - labels.at<float>(i, 0), 2);
            wrong++;
        }
    }
    fprintf(stderr, "Acc %f  %lu/%lu MSE %lu\n", 1.0 * right/(right + wrong), right, right + wrong, mse);
}

extern "C" void *train_cv_tree(mtm_t *mtm) {
    cv::Mat samples_mtm = build_samples_from_mtm(mtm);
    cv::Mat labels_mtm = build_classes_from_mtm(mtm);
    CvDTreeParams params;
    params.truncate_pruned_tree = false;
    params.min_sample_count = 1;
    params.cv_folds = 1;
    params.use_1se_rule = false;
    params.use_surrogates = false;
    params.max_categories = INT_MAX;
    cv::Mat var_type(1, mtm->wsize + 1, CV_8UC1);
    for (int i = 0; i < mtm->wsize + 1; i++) {
        var_type.at<unsigned char>(0, i) = CV_VAR_ORDERED;
    }
    CvDTree *tr_mtm = new CvDTree();
    tr_mtm->pruned_tree_idx = -INT_MAX;
    tr_mtm->train(samples_mtm, CV_ROW_SAMPLE, labels_mtm, cv::Mat(), cv::Mat(), var_type, cv::Mat(), params);
    test_accuracy(*tr_mtm, samples_mtm, labels_mtm);

    return (void *) tr_mtm;
}

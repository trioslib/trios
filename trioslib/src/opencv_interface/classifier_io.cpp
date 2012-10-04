#include <cstdio>

#include "opencv_interface.h"

#include "opencv2/opencv.hpp"
#include "opencv2/ml/ml.hpp"


extern "C" dTree *read_cv_tree(char *filename) {
    CvDTree *tree = new CvDTree();
    tree->load(filename);
    return (dTree *)tree;
}

extern "C" int write_cv_tree(char *filename, dTree *tree) {
    CvDTree *cv_tree = (CvDTree *)tree;
    cv_tree->save(filename);
    return 1;
}

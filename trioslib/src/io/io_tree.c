#include "../opencv_interface/opencv_interface.h"

dTree *read_tree2(char *filename) {
    return read_cv_tree(filename);
}

void write_tree2(char *filename, dTree *tree) {
    write_cv_tree(filename, tree);
}

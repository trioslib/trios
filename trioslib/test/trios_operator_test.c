#include "trios.h"
#include "trios_operator.h"

#include "minunit.h"

UTEST(BUILD_BB) {
    image_operator_t *iop;
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input3.pgm");
    imgset_set_fname(set, 2, 1, "ideal3.pgm");

    window_t *w3x3 = win_create(3, 3, 1);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            win_set_point(i, j, 1, 1, w3x3);
        }
    }

    iop = image_operator_build_bb(set, w3x3);


} TEST_END

UTEST(APPLY_BB) {
    image_operator_t *iop;
    img_t *output, *input;
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input3.pgm");
    imgset_set_fname(set, 2, 1, "ideal3.pgm");

    window_t *w3x3 = win_create(10, 4, 1);
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, w3x3);
        }
    }

    iop = image_operator_build_bb(set, w3x3);
    input = img_readPGM("./test_img/input3.pgm");
    output = image_operator_apply(iop, input);
    img_writePGM("operator_apply_BB.pgm", output);
} TEST_END

UTEST(BUILD_GG) {
    image_operator_t *iop;
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1-einstein.pnm");
    /*imgset_set_fname(set, 1, 2, "input2-einstein.pnm");*/
    imgset_set_fname(set, 2, 1, "ideal-einstein.pnm");
    /*imgset_set_fname(set, 2, 2, "ideal-einstein.pnm");*/

    window_t *w3x3 = win_create(3, 3, 1);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            win_set_point(i, j, 1, 1, w3x3);
        }
    }

    iop = image_operator_build_gg(set, w3x3);


} TEST_END

UTEST(APPLY_GG) {
    image_operator_t *iop;
    img_t *output, *input;
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input2-einstein.pnm");
    imgset_set_fname(set, 2, 1, "ideal-einstein.pnm");

    window_t *w3x3 = win_create(5, 5, 1);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, w3x3);
        }
    }

    iop = image_operator_build_gg(set, w3x3);
    input = img_readPGM("./test_img/input2-einstein.pnm");
    output = image_operator_apply(iop, input);
    img_writePGM("operator_apply_GG.pgm", output);
} TEST_END

UTEST(IO_BB) {
    image_operator_t *iop;
    img_t *output, *input;
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input3.pgm");
    imgset_set_fname(set, 2, 1, "ideal3.pgm");

    window_t *w3x3 = win_create(10, 4, 1);
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, w3x3);
        }
    }

    iop = image_operator_build_bb(set, w3x3);

    image_operator_write("iop_bb", iop);

    input = img_readPGM("./test_img/input3.pgm");
    output = image_operator_apply(iop, input);
    img_writePGM("operator_apply_IO_BB.pgm", output);
} TEST_END

UTEST(IO_GG) {
    image_operator_t *iop;
    img_t *output, *input;
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input2-einstein.pnm");
    imgset_set_fname(set, 2, 1, "ideal-einstein.pnm");

    window_t *w3x3 = win_create(5, 5, 1);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, w3x3);
        }
    }

    iop = image_operator_build_gg(set, w3x3);

    image_operator_write("iop_gg", iop);

    input = img_readPGM("./test_img/input2-einstein.pnm");
    output = image_operator_apply(iop, input);
    img_writePGM("operator_apply_GG.pgm", output);
} TEST_END


#include "runner.h"

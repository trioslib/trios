#include "minunit.h"
#include <trios.h>

UTEST(STRUCTURES_CREATION) {
    int levels[] = {3, 1};
    multi_architecture_t *arch = multi_level_arch_create(2, levels);

    window_t *win = win_create(3, 3, 1);

    multi_level_arch_set_window(arch, 0, 0, 0, win);
    multi_level_arch_set_window(arch, 0, 1, 0, win);
    multi_level_arch_set_window(arch, 0, 2, 0, win);

    multi_level_arch_set_window(arch, 1, 0, 0, win);
    multi_level_arch_set_window(arch, 1, 0, 1, win);
    multi_level_arch_set_window(arch, 1, 0, 2, win);

    mu_assert("Ninputs wrong for level 0", arch->levels[0].ninputs == 1);
    multi_level_arch_free(arch);
    win_free(win);
} TEST_END

UTEST(STRUCTURES_CREATION2) {
    int levels[] = {3, 1};
    multi_architecture_t *arch = multi_level_arch_create(2, levels);

    window_t *win = win_create(3, 3, 1);

    multi_level_arch_set_window(arch, 0, 0, 0, win);
    multi_level_arch_set_window(arch, 0, 1, 0, win);
    multi_level_arch_set_window(arch, 0, 2, 0, win);

    multi_level_arch_set_window(arch, 1, 0, 0, win);
    multi_level_arch_set_window(arch, 1, 0, 1, win);
    multi_level_arch_set_window(arch, 1, 0, 2, win);

    multi_level_operator_t *op = multi_level_operator_create(arch, BB);

    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    win_free(win);
} TEST_END

xpl_t *collec_level_operator_bb_main(multi_level_operator_t *mop, int level, int op, img_t **inputs, img_t *mask, img_t *ideal);

UTEST(COLLEC1) {
    int levels[] = {1};
    int i, j;
    multi_architecture_t *arch = multi_level_arch_create(1, levels);

    window_t *win = win_create(3, 3, 1);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    multi_level_arch_set_window(arch, 0, 0, 0, win);

    multi_level_operator_t *op = multi_level_operator_create(arch, BB);
    img_t *input = img_readPGM("./test_img/input1.pgm");
    img_t *ideal = img_readPGM("./test_img/ideal1.pgm");
    xpl_t *xpl = collec_level_operator_bb_main(op, 0, 0, &input, NULL, ideal);

    xpl_write("multi.xpl", xpl, win, NULL);

    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    img_free(input);
    img_free(ideal);
    xpl_free(xpl);
    win_free(win);
} TEST_END

UTEST(COLLEC2) {
    int levels[] = {2, 1};
    int i, j;
    multi_architecture_t *arch = multi_level_arch_create(2, levels);

    window_t *win = win_create(3, 3, 1);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }


    multi_level_arch_set_window(arch, 0, 0, 0, win);
    multi_level_arch_set_window(arch, 0, 1, 0, win);

    multi_level_arch_set_window(arch, 1, 0, 0, win);
    multi_level_arch_set_window(arch, 1, 0, 1, win);

    multi_level_operator_t *op = multi_level_operator_create(arch, BB);
    img_t *ideal = img_readPGM("./test_img/ideal1.pgm");

    img_t *inputs[2];
    inputs[0] = img_readPGM("./test_img/input1.pgm");
    inputs[1] = img_readPGM("./test_img/input1.pgm");

    xpl_t *xpl = collec_level_operator_bb_main(op, 1, 0, inputs, NULL, ideal);

    xpl_write("multi2.xpl", xpl, win, NULL);

    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    img_free(inputs[0]);
    img_free(inputs[1]);
    img_free(ideal);
    xpl_free(xpl);
    win_free(win);
} TEST_END

UTEST(BUILD1) {
    int levels[] = {1};
    int i, j;
    multi_architecture_t *arch = multi_level_arch_create(1, levels);

    window_t *win = win_create(3, 3, 1);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    multi_level_arch_set_window(arch, 0, 0, 0, win);

    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");

    multi_level_operator_t *op = multi_level_build_single(arch, set);

    imgset_free(set);
    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    win_free(win);
} TEST_END


UTEST(BUILD2) {
    int levels[] = {2, 1};
    int i, j;
    multi_architecture_t *arch = multi_level_arch_create(2, levels);

    window_t *win1 = win_create(3, 3, 1);
    for (i = 1; i < 3; i++) {
        for (j = 1; j < 3; j++) {
            win_set_point(i, j, 1, 1, win1);
        }
    }
    multi_level_arch_set_window(arch, 0, 0, 0, win1);

    window_t *win2 = win_create(3, 3, 1);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            win_set_point(i, j, 1, 1, win2);
        }
    }
    multi_level_arch_set_window(arch, 0, 1, 0, win2);

    window_t *win3 = win_create(1, 1, 1);
    win_set_point(0, 0, 1, 1, win3);


    multi_level_arch_set_window(arch, 1, 0, 0, win3);
    multi_level_arch_set_window(arch, 1, 0, 1, win3);

    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");

    multi_level_operator_t *op = multi_level_build_single(arch, set);

    imgset_free(set);
    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    win_free(win1);
    win_free(win2);
    win_free(win3);
} TEST_END


UTEST(APPLY1) {
    int levels[] = {2, 1};
    int i, j;
    multi_architecture_t *arch = multi_level_arch_create(2, levels);

    window_t *win1 = win_create(3, 3, 1);
    for (i = 1; i < 3; i++) {
        for (j = 1; j < 3; j++) {
            win_set_point(i, j, 1, 1, win1);
        }
    }
    multi_level_arch_set_window(arch, 0, 0, 0, win1);

    window_t *win2 = win_create(3, 3, 1);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            win_set_point(i, j, 1, 1, win2);
        }
    }
    multi_level_arch_set_window(arch, 0, 1, 0, win2);

    window_t *win3 = win_create(1, 1, 1);
    win_set_point(0, 0, 1, 1, win3);


    multi_level_arch_set_window(arch, 1, 0, 0, win3);
    multi_level_arch_set_window(arch, 1, 0, 1, win3);

    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");

    multi_level_operator_t *op = multi_level_build_single(arch, set);
    img_t *input = img_readPGM("test_img/input1.pgm");
    img_t *result = multi_level_apply(op, input, NULL);
    img_writePGM("result.pgm", result);

    imgset_free(set);
    img_free(input);
    img_free(result);
    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    win_free(win1);
    win_free(win2);
    win_free(win3);
} TEST_END

UTEST(APPLY2) {
    int levels[] = {2, 1};
    int i, j;
    multi_architecture_t *arch = multi_level_arch_create(2, levels);

    window_t *win1 = win_create(5, 5, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, win1);
        }
    }
    multi_level_arch_set_window(arch, 0, 0, 0, win1);

    window_t *win2 = win_create(5, 5, 1);
    for (i = 1; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, win2);
        }
    }
    multi_level_arch_set_window(arch, 0, 1, 0, win2);

    window_t *win3 = win_create(1, 1, 1);
    win_set_point(0, 0, 1, 1, win3);


    multi_level_arch_set_window(arch, 1, 0, 0, win3);
    multi_level_arch_set_window(arch, 1, 0, 1, win3);

    imgset_t *set1 = imgset_create(1, 2);
    imgset_set_dname(set1, 1, "./test_img/");
    imgset_set_dname(set1, 2, "./test_img/");
    imgset_set_fname(set1, 1, 1, "input1.pgm");
    imgset_set_fname(set1, 2, 1, "ideal1.pgm");

    imgset_t *set2 = imgset_create(1, 2);
    imgset_set_dname(set2, 1, "./test_img/");
    imgset_set_dname(set2, 2, "./test_img/");
    imgset_set_fname(set2, 1, 1, "input1.pgm");
    imgset_set_fname(set2, 2, 1, "ideal1.pgm");

    imgset_t *sets[] = {set1, set1};

    multi_level_operator_t *op = multi_level_build_bb(arch, sets);
    img_t *input = img_readPGM("test_img/input1.pgm");
    img_t *result = multi_level_apply(op, input, NULL);
    img_writePGM("result.pgm", result);

    imgset_free(set1);
    imgset_free(set2);
    img_free(input);
    img_free(result);
    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    win_free(win1);
    win_free(win2);
    win_free(win3);
} TEST_END

UTEST(BUILD_GG) {
    int levels[] = {2, 1};
    int i, j;
    multi_architecture_t *arch = multi_level_arch_create(2, levels);

    window_t *win1 = win_create(5, 5, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, win1);
        }
    }
    multi_level_arch_set_window(arch, 0, 0, 0, win1);

    window_t *win2 = win_create(5, 5, 1);
    for (i = 1; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, win2);
        }
    }
    multi_level_arch_set_window(arch, 0, 1, 0, win2);

    window_t *win3 = win_create(1, 1, 1);
    win_set_point(0, 0, 1, 1, win3);


    multi_level_arch_set_window(arch, 1, 0, 0, win3);
    multi_level_arch_set_window(arch, 1, 0, 1, win3);

    imgset_t *set1 = imgset_create(1, 2);
    imgset_set_dname(set1, 1, "./test_img/");
    imgset_set_dname(set1, 2, "./test_img/");
    imgset_set_fname(set1, 1, 1, "input1.pgm");
    imgset_set_fname(set1, 2, 1, "ideal1.pgm");

    imgset_t *set2 = imgset_create(1, 2);
    imgset_set_dname(set2, 1, "./test_img/");
    imgset_set_dname(set2, 2, "./test_img/");
    imgset_set_fname(set2, 1, 1, "input2.pgm");
    imgset_set_fname(set2, 2, 1, "ideal2.pgm");

    imgset_t *sets[] = {set1, set1};

    multi_level_operator_t *op = multi_level_build_gg(arch, sets);
    img_t *input = img_readPGM("test_img/input1.pgm");
    img_t *result = multi_level_apply(op, input, NULL);
    img_writePGM("resultGG.pgm", result);

    imgset_free(set1);
    imgset_free(set2);
    img_free(input);
    img_free(result);
    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    win_free(win1);
    win_free(win2);
    win_free(win3);
} TEST_END

UTEST(ARCH_IO) {
    int levels[] = {2, 1};
    int i, j, k, l, m;
    multi_architecture_t *arch = multi_level_arch_create(2, levels);

    window_t *win1 = win_create(3, 3, 1);
    for (i = 1; i < 3; i++) {
        for (j = 1; j < 3; j++) {
            win_set_point(i, j, 1, 1, win1);
        }
    }
    multi_level_arch_set_window(arch, 0, 0, 0, win1);

    window_t *win2 = win_create(3, 3, 1);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            win_set_point(i, j, 1, 1, win2);
        }
    }
    multi_level_arch_set_window(arch, 0, 1, 0, win2);

    window_t *win3 = win_create(1, 1, 1);
    win_set_point(0, 0, 1, 1, win3);


    multi_level_arch_set_window(arch, 1, 0, 0, win3);
    multi_level_arch_set_window(arch, 1, 0, 1, win3);

    mu_assert("Write failed", 1==multi_architecture_write("march1", arch));
    multi_architecture_t *read = multi_architecture_read("march1");

    mu_assert("Read failed", read != NULL);

    mu_assert("nlevels error", read->nlevels == arch->nlevels);
    mu_assert("nlevels error", read->levels[0].ninputs == arch->levels[0].ninputs);
    mu_assert("nlevels error", read->levels[0].noperators == arch->levels[0].noperators);
    mu_assert("nlevels error", read->levels[1].ninputs == arch->levels[1].ninputs);
    mu_assert("nlevels error", read->levels[1].noperators == arch->levels[1].noperators);
    for (k = 0; k < read->nlevels; k++) {
        for (i = 0; i < read->levels[k].noperators; i++) {
            for (j = 0; j < read->levels[k].ninputs; j++) {
                mu_assert("Win dimensions error",
                          win_get_width(read->levels[k].windows[i][j]) == win_get_width(arch->levels[k].windows[i][j])
                          && win_get_height(read->levels[k].windows[i][j]) == win_get_height(arch->levels[k].windows[i][j])
                          );
                for (l = 0; l < win_get_height(read->levels[k].windows[i][j]); l++) {
                    for (m = 0; m < win_get_width(read->levels[k].windows[i][j]); m++) {
                        mu_assert("Point error",
                                  win_get_point(l, m, 1, read->levels[k].windows[i][j]) ==
                                  win_get_point(l, m, 1, arch->levels[k].windows[i][j]));
                    }
                }

            }
        }
    }
} TEST_END

UTEST(OP_IO) {
    int levels[] = {2, 1};
    int i, j, k, l, m;
    multi_architecture_t *arch = multi_level_arch_create(2, levels);
    window_t *win1 = win_create(3, 3, 1);
    for (i = 1; i < 3; i++) {
        for (j = 1; j < 3; j++) {
            win_set_point(i, j, 1, 1, win1);
        }
    }
    multi_level_arch_set_window(arch, 0, 0, 0, win1);
    window_t *win2 = win_create(3, 3, 1);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            win_set_point(i, j, 1, 1, win2);
        }
    }
    multi_level_arch_set_window(arch, 0, 1, 0, win2);
    window_t *win3 = win_create(1, 1, 1);
    win_set_point(0, 0, 1, 1, win3);
    multi_level_arch_set_window(arch, 1, 0, 0, win3);
    multi_level_arch_set_window(arch, 1, 0, 1, win3);

    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");

    multi_level_operator_t *mop = multi_level_build_single(arch, set);
    mu_assert("Write failed", 1 == multi_level_operator_write("mop1", mop));

    multi_level_operator_t *read = multi_level_operator_read("mop1");

    img_t *input = img_readPGM("test_img/input1.pgm");
    img_t *result = multi_level_apply(mop, input, NULL);
    img_t *result2 = multi_level_apply(read, input, NULL);

    for (i = 0; i < img_get_height(result); i++) {
        for (j = 0; j < img_get_width(result); j++) {
            mu_assert("Result error",
                      img_get_pixel(result, i, j, 0) ==
                      img_get_pixel(result2, i, j, 0));
        }
    }

} TEST_END


UTEST(BUILD_MULTI) {
    int levels[] = {2, 2, 1};
    int i, j;
    multi_architecture_t *arch = multi_level_arch_create(3, levels);

    window_t *win1 = win_create(3, 3, 1);
    for (i = 1; i < 3; i++) {
        for (j = 1; j < 3; j++) {
            win_set_point(i, j, 1, 1, win1);
        }
    }
    multi_level_arch_set_window(arch, 0, 0, 0, win1);

    window_t *win2 = win_create(3, 3, 1);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            win_set_point(i, j, 1, 1, win2);
        }
    }
    multi_level_arch_set_window(arch, 0, 1, 0, win2);

    multi_level_arch_set_window(arch, 1, 0, 0, win1);
    multi_level_arch_set_window(arch, 1, 0, 1, win2);

    multi_level_arch_set_window(arch, 1, 1, 0, win2);
    multi_level_arch_set_window(arch, 1, 1, 1, win1);


    window_t *win3 = win_create(1, 1, 1);
    win_set_point(0, 0, 1, 1, win3);


    multi_level_arch_set_window(arch, 2, 0, 0, win3);
    multi_level_arch_set_window(arch, 2, 0, 1, win3);

    imgset_t *set1 = imgset_create(1, 2);
    imgset_set_dname(set1, 1, "./test_img/");
    imgset_set_dname(set1, 2, "./test_img/");
    imgset_set_fname(set1, 1, 1, "input1.pgm");
    imgset_set_fname(set1, 2, 1, "ideal1.pgm");

    imgset_t *set2 = imgset_create(1, 2);
    imgset_set_dname(set2, 1, "./test_img/");
    imgset_set_dname(set2, 2, "./test_img/");
    imgset_set_fname(set2, 1, 1, "input2.pgm");
    imgset_set_fname(set2, 2, 1, "ideal2.pgm");

    imgset_t *sets[] = {set1, set2, set1};

    multi_level_operator_t *op = multi_level_build_bb(arch, sets);

    img_t *input = img_readPGM("test_img/input1.pgm");
    img_t *result = multi_level_apply(op, input, NULL);
    img_writePGM("multi_imgset.pgm", result);

    imgset_free(set1);
    imgset_free(set2);
    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    win_free(win1);
    win_free(win2);
    win_free(win3);
} TEST_END


UTEST(TEST_COMBINE) {
    itv_t *itvs[5];
    window_t *wins[5];
    image_operator_t *ops[5];
    imgset_t *set;
    multi_level_operator_t *mop;
    int i;

    itvs[0] = itv_read("jung9x7/bas1.1.bas", wins);
    itvs[1] = itv_read("jung9x7/bas1.2.bas", wins+1);
    itvs[2] = itv_read("jung9x7/bas1.3.bas", wins+2);
    itvs[3] = itv_read("jung9x7/bas1.4.bas", wins+3);
    itvs[4] = itv_read("jung9x7/bas1.5.bas", wins+4);

    for (i = 0; i < 5; i++)
        ops[i] = image_operator_itv(itvs[i], wins[i]);

    set = imgset_read("jung9x7/level2.set");

    mop = multi_level_combine_operators(ops, 5, set);
    itv_write("jung9x7/test.itv", (itv_t *) mop->levels[1].trained_operator[0], multi_level_operator_joint_window(mop, 1, 0));

    system("cat jung9x7/test.itv jung9x7/base.bas");

    multi_level_operator_free(mop);
    /*itv_free(itvs[0]);
    itv_free(itvs[1]);
    itv_free(itvs[2]);
    itv_free(itvs[3]);
    itv_free(itvs[4]);*/
    win_free(wins[0]);
    win_free(wins[1]);
    win_free(wins[2]);
    win_free(wins[3]);
    win_free(wins[4]);
    imgset_free(set);

} TEST_END

#include "runner.h"


#include <trios.h>
#include "trios_appl.h"
#include "minunit.h"

UTEST(test_decision_pair1) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1.pgm");
    imgset_set_fname(set, 2, 1, "ideal1.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, BB, 0, 1, 0);

    itv_write("ITV_TEST.itv", itv, win);
    itv_free(itv);
    win_free(win);


    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    mu_assert("ldecision failed", 1 == ldecision_disk("XPL_RESULT.xpl", 1, 0, AVERAGE, 0, 0, "DECISION_RESULT.mtm"));
    mu_assert("lisi_disk failed", 1 == lisi_disk("DECISION_RESULT.mtm", "ITV_TEST.itv", 3, 5, 0, 0, "ITV_MIN.itv", 0, NULL, NULL ));
    mu_assert("lpapplic_disk failed", 1 == lpapplic_disk("./test_img/input1.pgm", "ITV_MIN.itv", NULL, 0, 0, 255, "./test_img/test1.pgm"));
} TEST_END

UTEST(test_decision_pair2) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input2.pgm");
    imgset_set_fname(set, 2, 1, "ideal2.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, BB, 0, 1, 0);

    itv_write("ITV_TEST.itv", itv, win);
    itv_free(itv);
    win_free(win);


    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    mu_assert("ldecision failed", 1 == ldecision_disk("XPL_RESULT.xpl", 1, 0, AVERAGE, 0, 0, "DECISION_RESULT.mtm"));
    mu_assert("lisi_disk failed", 1 == lisi_disk("DECISION_RESULT.mtm", "ITV_TEST.itv", 3, 5, 0, 0, "ITV_MIN.itv", 0, NULL, NULL ));
    mu_assert("lpapplic_disk failed", 1 == lpapplic_disk("./test_img/input2.pgm", "ITV_MIN.itv", NULL, 0, 0, 255, "./test_img/test2.pgm"));
} TEST_END

UTEST(test_decision_pair3) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input3.pgm");
    imgset_set_fname(set, 2, 1, "ideal3.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, BB, 0, 1, 0);

    itv_write("ITV_TEST.itv", itv, win);
    itv_free(itv);
    win_free(win);


    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    mu_assert("ldecision failed", 1 == ldecision_disk("XPL_RESULT.xpl", 1, 0, AVERAGE, 0, 0, "DECISION_RESULT.mtm"));
    mu_assert("lisi_disk failed", 1 == lisi_disk("DECISION_RESULT.mtm", "ITV_TEST.itv", 3, 5, 0, 0, "ITV_MIN.itv", 0, NULL, NULL ));
    mu_assert("lpapplic_disk failed", 1 == lpapplic_disk("./test_img/input3.pgm", "ITV_MIN.itv", NULL, 0, 0, 255, "./test_img/test3.pgm"));
} TEST_END

UTEST(test_apply_in_memory) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input3.pgm");
    imgset_set_fname(set, 2, 1, "ideal3.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    itv_t *itv = itv_gen_itv(win, 1, BB, 0, 1, 0);
    win_free(win);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 1, 1, 0, "XPL_RESULT.xpl", NULL));
    xpl_t *xpl = xpl_read("XPL_RESULT.xpl", &win, NULL);
    mtm_t *mtm = ldecision_memory(xpl, 1, 0, AVERAGE, 0, 0);
    mu_assert("ldecision failed", mtm != NULL);
    itv = lisi_memory(mtm, itv, 3, 5, 0, 0);
    mu_assert("lisi failed", itv != NULL);

    img_t *input = img_readPGM("./test_img/input3.pgm");
    img_t *output = lpapplic_memory(input, itv, win, NULL, 0, 0, 255);
    mu_assert("lpapplic_memory failed", NULL != output);

    img_writePGM("memory.pgm", output);
    img_free(output);

    itv_free(itv);
    win_free(win);

} TEST_END


UTEST(test_applic_gg) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "small.pgm");
    imgset_set_fname(set, 2, 1, "small-ideal.pgm");
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(4, 4, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);

    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 0, 0, 0, "XPL_RESULT_GG1.xpl", NULL));
    xpl_t *xpl = xpl_read("XPL_RESULT_GG1.xpl", &win, NULL);
    mtm_t *mtm = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);
    mtm_write("mtm_gg1.mtm", mtm, win, NULL);
    dTree *gg = ltrainGG_memory(mtm);
    img_t *input = img_readPGM("./test_img/small.pgm");
    img_t *out = lapplyGG_memory(input, gg, win);
    img_writePGM("small_gg_res.pgm", out);
    /*ltrainDT("mtm_gg1.mtm", 0, 0, 0, 0, NULL, "tree_gg1");
    printf("SDFSDFS\n\n\n");
    lapplicDT("./test_img/input1_GG.pnm", "tree_gg1", NULL, "gg_res.pgm");*/
    win_free(win);
} TEST_END

UTEST(test_applic_gg_einstein) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1-einstein.pnm");
    /*imgset_set_fname(set, 1, 2, "input2-einstein.pnm");*/
    imgset_set_fname(set, 2, 1, "ideal-einstein.pnm");
    /*imgset_set_fname(set, 2, 2, "ideal-einstein.pnm");*/
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(5, 5, 1);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);
    printf("Collec\n");
    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 0, 0, 0, "XPL_RESULT_GG1.xpl", NULL));
    xpl_t *xpl = xpl_read("XPL_RESULT_GG1.xpl", &win, NULL);
    printf("Decision\n");
    mtm_t *mtm = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);
    dTree *bb = ltrainGG_memory(mtm);
    img_t *in = img_readPGM("./test_img/input2-einstein.pnm");
    img_t *out = lapplyGG_memory(in, bb, win);
    img_writePGM("gg_res_cv_einstein.pgm", out);


    win_free(win);
} TEST_END

UTEST(test_applic_gg_einstein_io) {
    int i, j;
    imgset_t *set = imgset_create(1, 2);
    imgset_set_dname(set, 1, "./test_img/");
    imgset_set_dname(set, 2, "./test_img/");
    imgset_set_fname(set, 1, 1, "input1-einstein.pnm");
    /*imgset_set_fname(set, 1, 2, "input2-einstein.pnm");*/
    imgset_set_fname(set, 2, 1, "ideal-einstein.pnm");
    /*imgset_set_fname(set, 2, 2, "ideal-einstein.pnm");*/
    imgset_write("IMGSET.s", set);
    imgset_free(set);

    window_t *win = win_create(5, 5, 1);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            win_set_point(i, j, 1, 1, win);
        }
    }
    win_write("WIN.w", win);
    mu_assert("lcollec failed.", 1 == lcollec("IMGSET.s", "WIN.w", NULL, 0, 0, 0, "XPL_RESULT_GG1.xpl", NULL));
    xpl_t *xpl = xpl_read("XPL_RESULT_GG1.xpl", &win, NULL);
    mtm_t *mtm = ldecision_memory(xpl, 0, 0, AVERAGE, 0, 0);
    mtm_write("gg.mtm", mtm, win, NULL);
    dTree *bb = ltrainGG_memory(mtm);
    write_tree2("tree2", bb);
    dTree *gg = read_tree2("tree2");

    img_t *in = img_readPGM("./test_img/input2-einstein.pnm");
    img_t *out = lapplyGG_memory(in, gg, win);
    img_writePGM("gg_res_cv_einstein.pgm", out);

    win_free(win);
} TEST_END

#include "runner.h"

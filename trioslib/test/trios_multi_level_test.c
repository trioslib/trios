#include "minunit.h"
#include <trios.h>

TEST(STRUCTURES_CREATION) {
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

TEST(STRUCTURES_CREATION2) {
    int levels[] = {3, 1};
    multi_architecture_t *arch = multi_level_arch_create(2, levels);

    window_t *win = win_create(3, 3, 1);

    multi_level_arch_set_window(arch, 0, 0, 0, win);
    multi_level_arch_set_window(arch, 0, 1, 0, win);
    multi_level_arch_set_window(arch, 0, 2, 0, win);

    multi_level_arch_set_window(arch, 1, 0, 0, win);
    multi_level_arch_set_window(arch, 1, 0, 1, win);
    multi_level_arch_set_window(arch, 1, 0, 2, win);

    multi_level_operator_t *op = multi_level_operator_create(arch);

    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    win_free(win);
} TEST_END

TEST(COLLEC1) {
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

    multi_level_operator_t *op = multi_level_operator_create(arch);
    img_t *input = img_readPGM("./test_img/input1.pgm");
    img_t *ideal = img_readPGM("./test_img/ideal1.pgm");
    xpl_t *xpl = collec_level_operator_bb_main(op, 0, 0, &input, ideal);

    xpl_write("multi.xpl", xpl, win);

    multi_level_arch_free(arch);
    multi_level_operator_free(op);
    img_free(input);
    img_free(ideal);
    xpl_free(xpl);
    win_free(win);
} TEST_END



#include "runner.h"

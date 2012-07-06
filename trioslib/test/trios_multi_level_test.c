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


    multi_level_arch_free(arch);
    win_free(win);
} TEST_END

#include "runner.h"

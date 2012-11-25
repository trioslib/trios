#include <trios.h>

int main(int argc, char *argv[]) {
    image_operator_t *op;
    window_t *win;
    imgset_t *training;

    if (argc < 5) {
        printf("TRIOS tools: build\n");
        printf("Usage: $trios-build type window(s) image_set path_to_write \n");
        return -1;
    }

    if (strcmp(argv[1], "BB") == 0) {
        win = win_read(argv[2]);
        if (!win) {
            printf("Invalid window.\n");
            return -1;
        }

        training = imgset_read(argv[3]);
        if (!training) {
            printf("Invalid image set.\n");
            return -1;
        }

        op = image_operator_build_bb(training, win);
        if (op == NULL) {
            printf("Error training operator.\n");
            return -1;
        }

        image_operator_write(argv[4], op);
    } else if (strcmp(argv[1], "BB-two-level") == 0) {
        int i;
        multi_level_operator_t *mop;
        multi_architecture_t *arch;
        int levels[] = {0, 1};
        window_t *two_level, *temp;
        levels[0] = argc - 4;
        arch = multi_level_arch_create(2, levels);
        two_level = win_create(1, 1, 1);
        win_set_point(0, 0, 1, 1, two_level);
        for (i = 2; i < argc - 2; i++) {
            temp = win_read(argv[i]);
            multi_level_arch_set_window(arch, 0, i-2, 0, temp);
            multi_level_arch_set_window(arch, 1, 0, i-2, two_level);
        }


        training = imgset_read(argv[i]);
        mop = multi_level_build(arch, training);
        multi_level_operator_write(argv[argc-1], mop);

    }
    return 0;
}

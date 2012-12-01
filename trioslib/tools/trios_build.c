#include <trios.h>

int main(int argc, char *argv[]) {
    image_operator_t *op;
    window_t *win;
    int i;
    imgset_t *training, *training2l[2];

    if (argc < 5) {
        printf("TRIOS tools: build\n");
        printf("Usage: $trios-build type window(s) image_set path_to_write \n");
        printf("Usage: $trios-build two-level-old bases path_to_write \n");
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
        levels[0] = argc - 5;
        arch = multi_level_arch_create(2, levels);
        two_level = win_create(1, 1, 1);
        win_set_point(0, 0, 1, 1, two_level);
        for (i = 2; i < argc - 3; i++) {
            temp = win_read(argv[i]);
            multi_level_arch_set_window(arch, 0, i-2, 0, temp);
            multi_level_arch_set_window(arch, 1, 0, i-2, two_level);
        }


        training2l[0] = imgset_read(argv[i]);
        training2l[1] = imgset_read(argv[i+1]);
        mop = multi_level_build(arch, training2l);
        multi_level_operator_write(argv[argc-1], mop);
    } else if (strcmp(argv[1], "two-level-old") == 0) {
        multi_architecture_t *arch;
        multi_level_operator_t *mop;
        window_t *two_level;
        int levels[] = {0, 1};
        int nops = argc - 3;
        levels[0] = nops - 1;
        arch = multi_level_arch_create(2, levels);
        two_level = win_create(1, 1, 1);
        win_set_point(0, 0, 1, 1, two_level);
        for (i = 2; i < argc - 2; i++) {
            printf("Set fake window %d: %s %d\n", i-2, argv[i], nops-1);
            multi_level_arch_set_window(arch, 0, i-2, 0, two_level);
            multi_level_arch_set_window(arch, 1, 0, i-2, two_level);
        }
        printf("Create\n");
        mop = multi_level_operator_create(arch);
        for (i = 0; i < nops-1; i++) {
            mop->levels[0].trained_operator[i] = itv_read(argv[i+2], &win);
            mop->levels[0].windows[i][0] = win;
        }
        mop->levels[1].trained_operator[0] = itv_read(argv[i+2], &win);

        multi_level_operator_write(argv[argc-1], mop);
    } else if (strcmp(argv[1], "two-level-merge-itv") == 0) {
        multi_level_operator_t *mop;
        window_t **wins;
        itv_t **itvs;
        int nops = argc - 4;

        trios_malloc(itvs, sizeof(itv_t *) * nops, int, "Error allocating itv_t array.");
        trios_malloc(wins, sizeof(window_t *) * nops, int, "Error allocating window_t array.");
        for (i = 0; i < nops; i++) {
            printf("read %s\n", argv[i+2]);
            itvs[i] = itv_read(argv[i+2], wins+i);
        }
        training = imgset_read(argv[argc-2]);

        mop = multi_level_combine_itv(itvs, wins, nops, training);

        multi_level_operator_write(argv[argc-1], mop);
    }

    return 0;
}

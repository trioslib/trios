#include <trios.h>

void print_usage() {
    printf("Usage: trios_build [single|two-level] [BB|GG] window(s) training_set [level2_training_set] result_path\n");
    printf("       trios_build join operator list level2_training_set result_path\n\n");
    printf("This tools executes the training process to learn image operators from a set of samples.\n");
    printf("The join option skips the first level training step and builds a two-level operator from trained single operators.\n");
}


void train_single_level(int argc, char *argv[]) {
    window_t *win;
    imgset_t *training;
    image_operator_t *op;

    if (argc < 6) {
        print_usage();
        return;
    }

    win = win_read(argv[3]);
    if (!win) {
        trios_fatal("Invalid window %s.\n", argv[3]);
    }

    training = imgset_read(argv[4]);
    if (!training) {
        trios_fatal("Invalid image set %s.\n", argv[4]);
    }

    if (strcmp(argv[2], "BB") == 0) {
        op = image_operator_build_bb(training, win);
    } else if (strcmp(argv[2], "GG") == 0) {
        op = image_operator_build_gg(training, win);
    } else {
        trios_fatal("Unknown operator type.");
    }

    if (op == NULL) {
        trios_fatal("Error during the training process.\n\n");
    }

    if (image_operator_write(argv[5], op) == 0) {
        trios_fatal("Error writing operator %s \n", argv[5]);
    }
}

void train_two_level(int argc, char *argv[]) {
    multi_architecture_t *arch;
    window_t *second_level, *temp;
    multi_level_operator_t *mop;
    imgset_t *training[2];
    int n_wins, i, ops_per_level[2];

    if (strcmp(argv[2], "GG") == 0) trios_fatal("Operation not implemented.");

    n_wins = argc - 6;
    if (n_wins < 2) {
        trios_fatal("Two level operators must have more than one window in the first level.");
    }
    ops_per_level[0] = n_wins; ops_per_level[1] = 1;
    arch = multi_level_arch_create(2, ops_per_level);
    second_level = win_create(1, 1, 1);
    win_set_point(0, 0, 1, 1, second_level);
    for (i = 3; i < argc - 3; i++) {
        temp = win_read(argv[i]);
        if (temp == NULL) trios_fatal("Error reading window %s", argv[i]);
        multi_level_arch_set_window(arch, 0, i-3, 0, temp);
        multi_level_arch_set_window(arch, 1, 0, i-3, second_level);
    }

    training[0] = imgset_read(argv[argc-3]);
    training[1] = imgset_read(argv[argc-2]);

    if (training[0] == NULL) trios_fatal("Error reading training set: %s.", argv[argc-3]);
    if (training[1] == NULL) trios_fatal("Error reading training set: %s.", argv[argc-2]);

    mop = multi_level_build(arch, training);
    if (mop == NULL) trios_fatal("Error building multi-level operator.");
    if (multi_level_operator_write(argv[argc-1]) == 0) {
        trios_fatal("Error writing operator %s.", argv[argc-1]);
    }
}

void join_two_level(int argc, char *argv[]) {
    trios_fatal("Operation not implemented.");
}

int main(int argc, char *argv[]) {
    image_operator_t *op;
    window_t *win;
    int i;
    imgset_t *training, *training2l[2];

    if (argc < 2) {
        print_usage();
        return -1;
    }

    if (strcmp(argv[1], "single") == 0) {
        train_single_level(argc, argv);
    } else if (strcmp(argv[1], "two-level") == 0) {
        train_two_level(argc, argv);
    } else if (strcmp(argv[1], "join") == 0) {
        join_two_level(argc, argv);
    } else {
        print_usage();
        return -1;
    }
    /*
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
    }*/

    return 0;
}

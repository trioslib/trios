#include <trios.h>
#include <stdio.h>

void print_usage() {
    fprintf(stderr, "Usage: trios_build [single|two-level] [BB|GG|GB] window(s) training_set [level2_training_set] result_path\n");
    fprintf(stderr, "       trios_build  single WK window aperture training_set result_path\n\n");
    fprintf(stderr, "       trios_build combine operator list level2_training_set result_path\n\n");
    fprintf(stderr, "       trios_build combine_xpl 2nd_xpl operator list result_path\n\n");

    fprintf(stderr, "This tools executes the training process to learn image operators from a set of samples.\n");
    fprintf(stderr, "The combine option skips the first level training step and builds a two-level operator from trained single operators.\n");
    fprintf(stderr, "The combine_xpl option builds a first level operator using an already observed set of patterns (2nd_xpl). 2nd_xpl can be obtained running trios_collec or by reobserving the patterns of a bigger domain using trios_xpl_util.\n");
}


void train_single_level(int argc, char *argv[]) {
    window_t *win;
    imgset_t *training;
    apert_t *apt;
    image_operator_t *op;
    int tr_set_index = 4;

    if (argc < 6) {
        print_usage();
        return;
    }

    win = win_read(argv[3]);
    if (!win) {
        trios_fatal("Invalid window %s.\n", argv[3]);
    }
    
    if (strcmp(argv[2], "WK") == 0) tr_set_index = 5;

    training = imgset_read(argv[tr_set_index]);
    if (!training) {
        trios_fatal("Invalid image set %s.\n", argv[tr_set_index]);
    }

    if (strcmp(argv[2], "BB") == 0) {
        op = image_operator_build_bb(training, win);
    } else if (strcmp(argv[2], "GG") == 0) {
        op = image_operator_build_gg(training, win);
    } else if (strcmp(argv[2], "GB") == 0) {
        op = image_operator_build_gb(training, win);
    } else if (strcmp(argv[2], "WK") == 0) {
        apt = apert_read(argv[4]);
        op = image_operator_build_wk(training, win, apt);
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
    int type;

    if (strcmp(argv[2], "BB") == 0) {
        type = BB;
    } else if (strcmp(argv[2], "GG") == 0) {
        type = GG;
    } else if (strcmp(argv[2], "GB") == 0) {
        type = GB;
    }

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

    
    mop = multi_level_build(arch, training, type);
    if (mop == NULL) trios_fatal("Error building multi-level operator.");
    if (multi_level_operator_write(argv[argc-1], mop) == 0) {
        trios_fatal("Error writing operator %s.", argv[argc-1]);
    }
}

int join_two_level(int argc, char *argv[]) {
    multi_level_operator_t *mop;
    image_operator_t **ops;
    imgset_t *training;
    window_t *wold;
    itv_t *iold;
    int i;
    int nops = argc - 4;

    trios_malloc(ops, sizeof(image_operator_t *) * nops, int, "Error allocating image_operator_t array.");
    for (i = 2; i < argc - 2; i++) {
        printf("Read %s\n", argv[i]);
        ops[i-2] = image_operator_read(argv[i]);
        if (ops[i-2] == NULL) {
            iold = itv_read(argv[i], &wold);
            if (iold == NULL) {
                trios_fatal("Could not read %s.", argv[i]);
            }
            ops[i-2] = image_operator_itv(iold, wold);
        }
    }
    training = imgset_read(argv[argc-2]);

    mop = multi_level_combine_operators(ops, nops, training);
    multi_level_operator_write(argv[argc-1], mop);

    return 1;
}

int combine_xpl(int argc, char *argv[]) {
    multi_level_operator_t *mop;
    image_operator_t **ops;
    xpl_t *collec2nd;
    window_t *win;
    itv_t *iold;
    int i;
    int nops = argc - 4;

    trios_malloc(ops, sizeof(image_operator_t *) * nops, int, "Error allocating image_operator_t array.");
    for (i = 3; i < argc - 1; i++) {
        printf("Read %s\n", argv[i]);
        ops[i-3] = image_operator_read(argv[i]);
        if (ops[i-3] == NULL) {
            iold = itv_read(argv[i], &win);
            if (iold == NULL) {
                trios_fatal("Could not read %s.", argv[i]);
            }
            ops[i-3] = image_operator_itv(iold, win);
        }
    }
    collec2nd = xpl_read(argv[2], &win, NULL);

    mop = multi_level_combine_xpl(ops, nops, collec2nd);
    multi_level_operator_write(argv[argc-1], mop);

    return 0;
}

int main(int argc, char *argv[]) {
    image_operator_t *op;
    window_t *win;
    int i;
    imgset_t *training, *training2l[2];

    if (argc < 2) {
        print_usage();
        return 0;
    }

    if (strcmp(argv[1], "single") == 0) {
        train_single_level(argc, argv);
    } else if (strcmp(argv[1], "two-level") == 0) {
        train_two_level(argc, argv);
    } else if (strcmp(argv[1], "combine") == 0) {
        join_two_level(argc, argv);
    } else if (strcmp(argv[1], "combine_xpl") == 0) {
        return combine_xpl(argc, argv);
    } else {
        print_usage();
        return 0;
    }

    return 0;
}

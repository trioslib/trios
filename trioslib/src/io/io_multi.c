#include "trios.h"

window_t *multi_level_operator_joint_window(multi_level_operator_t *mop, int level, int op);

int multi_architecture_write(char *filename, multi_architecture_t *arch) {
    int i, j, k;
    char window_name[1024];
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return trios_error(MSG, "Failed to open %s.", filename);
    }
    fprintf(f, "%d\n", arch->nlevels);
    for (k = 0; k < arch->nlevels; k++) {
        fprintf(f, "%d ", arch->levels[k].noperators);
    }
    fprintf(f, "\n");
    for (k = 0; k < arch->nlevels; k++) {
        fprintf(f, "%d %d\n", arch->levels[k].ninputs, arch->levels[k].noperators);
        for (i = 0; i < arch->levels[k].noperators; i++) {
            for (j = 0; j < arch->levels[k].ninputs; j++) {
                sprintf(window_name, "%s_win%dx%dx%d", filename, k, i, j);
                win_write(window_name, arch->levels[k].windows[i][j]);
                /*fprintf(f, "%s\n", window_name);*/
            }
        }
    }

    fclose(f);
    return 1;
}

multi_architecture_t * multi_architecture_read(char *filename) {
    int i, j, k;
    char window_name[1024];
    multi_architecture_t *march;
    int nlevels, *ops;
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return (multi_architecture_t *) trios_error(MSG, "Failed to open %s.", filename);
    }
    if (fscanf(f, "%d\n", &nlevels) != 1) {
        return (multi_architecture_t *) trios_error(MSG, "Invalid multi_architecture file: %s nlevels", filename);
    }
    trios_malloc(ops, sizeof(int) * nlevels, multi_architecture_t *,"Failed to alloc memory for ops in m_arch_read.");
    for (k = 0; k < nlevels; k++) {
        if (fscanf(f, "%d ", ops + k) != 1) {
            return (multi_architecture_t *) trios_error(MSG, "Invalid multi_architecture file: %s level", filename);
        }
    }
    march = multi_level_arch_create(nlevels, ops);
    for (k = 0; k < nlevels; k++) {
        if (fscanf(f, "%d %d\n", &(march->levels[k].ninputs), &(march->levels[k].noperators)) != 2) {
            return (multi_architecture_t *) trios_error(MSG, "Invalid multi_architecture file: %s level %d", filename, k);
        }
        for (i = 0; i < march->levels[k].noperators; i++) {
            for (j = 0; j < march->levels[k].ninputs; j++) {
                sprintf(window_name, "%s_win%dx%dx%d", filename, k, i, j);
                march->levels[k].windows[i][j] = win_read(window_name);
            }
        }
    }
    return march;
}

int multi_level_operator_write(char *filename, multi_level_operator_t *mop) {
    int i, j, k;
    char temp_name[1024];
    window_t *joint;
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return trios_error(MSG, "Failed to open %s.", filename);
    }
    fprintf(f, "%d\n", mop->nlevels);
    for (k = 0; k < mop->nlevels; k++) {
        fprintf(f, "%d ", mop->levels[k].noperators);
    }
    fprintf(f, "\n");
    for (k = 0; k < mop->nlevels; k++) {
        fprintf(f, "%d %d\n", mop->levels[k].ninputs, mop->levels[k].noperators);
        for (i = 0; i < mop->levels[k].noperators; i++) {
            for (j = 0; j < mop->levels[k].ninputs; j++) {
                sprintf(temp_name, "%s_win%dx%dx%d", filename, k, i, j);
                win_write(temp_name, mop->levels[k].windows[i][j]);

                joint = multi_level_operator_joint_window(mop, k, i);
                sprintf(temp_name, "%s_op%dx%dx%d", filename, k, i, j);
                if (mop->type == BB) {
                    itv_write(temp_name, (itv_t *) mop->levels[k].trained_operator[i], joint);
                }
                win_free(joint);
            }
        }
    }

    fclose(f);
    return 1;
}

multi_level_operator_t *multi_level_operator_read(char *filename) {
    int i, j, k;
    char temp_name[1024];
    multi_level_operator_t *mop;
    multi_architecture_t *arch;
    window_t *joint;

    arch = multi_architecture_read(filename);
    if (arch == NULL) {
        trios_error(MSG, "Invalid architecture file.");
        return NULL;
    }
    mop = multi_level_operator_create(arch, BB);


    for (k = 0; k < mop->nlevels; k++) {
        for (i = 0; i < mop->levels[k].noperators; i++) {
            for (j = 0; j < mop->levels[k].ninputs; j++) {
                sprintf(temp_name, "%s_op%dx%dx%d", filename, k, i, j);
                if  (mop->type == BB) {
                    mop->levels[k].trained_operator[i] = (classifier_t *) itv_read(temp_name, &joint);
                }
            }
        }
    }
    multi_level_arch_free(arch);
    return mop;
}


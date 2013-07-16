#include "trios.h"

window_t *multi_level_operator_joint_window(multi_level_operator_t *mop, int level, int op);

int write_multi_architecture_data(FILE *f, char *filename, multi_architecture_t *arch) {
    int i, j, k;
    char window_name[1024], cmd[1024];

    fprintf(f, "%d\n", arch->nlevels);
    for (k = 0; k < arch->nlevels; k++) {
        fprintf(f, "%d ", arch->levels[k].noperators);
    }
    fprintf(f, "\n");
    for (k = 0; k < arch->nlevels; k++) {
        fprintf(f, "%d %d\n", arch->levels[k].ninputs, arch->levels[k].noperators);
        for (i = 0; i < arch->levels[k].noperators; i++) {
            for (j = 0; j < arch->levels[k].ninputs; j++) {
                sprintf(cmd, "mkdir -p %s-files/level%d/operator%d-files", filename, k, i);
                system(cmd);

                if (k == 0) {
                    sprintf(window_name, "%s-files/level%d/operator%d-files/window", filename, k, i);
                } else {
                    sprintf(window_name, "%s-files/level%d/operator%d-files/window%d", filename, k, i, j);
                }

                win_write(window_name, arch->levels[k].windows[i][j]);
            }
        }
    }
    return 1;
}

int write_multi_architecture_data2(FILE *f, char *filename, multi_level_operator_t *op) {
    int i, j, k;
    char window_name[1024], cmd[1024];

    fprintf(f, "%d\n", op->nlevels);
    for (k = 0; k < op->nlevels; k++) {
        fprintf(f, "%d ", op->levels[k].noperators);
    }
    fprintf(f, "\n");
    for (k = 0; k < op->nlevels; k++) {
        fprintf(f, "%d %d\n", op->levels[k].ninputs, op->levels[k].noperators);
        for (i = 0; i < op->levels[k].noperators; i++) {
            for (j = 0; j < op->levels[k].ninputs; j++) {
                sprintf(cmd, "mkdir -p %s-files/level%d/operator%d-files", filename, k, i);
                system(cmd);

                if (k == 0) {
                    sprintf(window_name, "%s-files/level%d/operator%d-files/window", filename, k, i);
                } else {
                    sprintf(window_name, "%s-files/level%d/operator%d-files/window%d", filename, k, i, j);
                }
                win_write(window_name, op->levels[k].windows[i][j]);
                /*fprintf(f, "%s\n", window_name);*/
            }
        }
    }
    return 1;
}


int multi_architecture_write(char *filename, multi_architecture_t *arch) {
    FILE *f = fopen(filename, "w");
    int r;
    if (f == NULL) {
        return trios_error(MSG, "Failed to open %s.", filename);
    }
    r = write_multi_architecture_data(f,filename, arch);
    fclose(f);
    return r;
}

multi_architecture_t *read_multi_architecture_data(FILE *f, char *filename) {
    int i, j, k;
    char window_name[1024];
    int nlevels, *ops;
    multi_architecture_t *march;

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
                if (k == 0) {
                    sprintf(window_name, "%s-files/level%d/operator%d-files/window", filename, k, i);
                } else {
                    sprintf(window_name, "%s-files/level%d/operator%d-files/window%d", filename, k, i, j);
                }
                march->levels[k].windows[i][j] = win_read(window_name);
            }
        }
    }
    return march;
}

multi_architecture_t * multi_architecture_read(char *filename) {
    multi_architecture_t *march;
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return (multi_architecture_t *) trios_error(MSG, "Failed to open %s.", filename);
    }
    march = read_multi_architecture_data(f, filename);
    return march;
}

int multi_level_operator_write(char *filename, multi_level_operator_t *mop) {
    int i, j, k;
    char temp_name[1024], cmd[1024];
    window_t *joint;
    FILE *first;
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return trios_error(MSG, "Failed to open %s.", filename);
    }
    if (mop->type == BB) {
        fprintf(f, "BB\n");
    } else if (mop->type == GG) {
        fprintf(f, "GG\n");
    }

    write_multi_architecture_data2(f, filename, mop);

    for (k = 0; k < mop->nlevels; k++) {
        for (i = 0; i < mop->levels[k].noperators; i++) {
            for (j = 0; j < mop->levels[k].ninputs; j++) {
                sprintf(temp_name, "%s-files/level%d/operator%d-files/operator", filename, k, i);
                if (mop->type == BB) {
                    joint = multi_level_operator_joint_window(mop, k, i);
                    itv_write(temp_name, (itv_t *) mop->levels[k].trained_operator[i], joint);
                    win_free(joint);
                    if (k == 0) {
                        /* write the operator file so that first level operators can be read as image_operator_t. */
                        sprintf(temp_name, "%s-files/level%d/operator%d", filename, k, i);
                        first = fopen(temp_name, "w");
                        if (first == NULL) return trios_error(MSG, "Failed to open %s.", temp_name);
                        fprintf(first, ".t\nBB\n");
                        fprintf(first, ".w\noperator%d-files/window\n", i);
                        fprintf(first, ".xpl\noperator%d-files/colled\n", i);
                        fprintf(first, ".mtm\noperator%d-files/decision\n", i);
                        fprintf(first, ".bb\noperator%d-files/operator\n", i);
                        fclose(first);
                    }
                } else if (mop->type == GG) {
                    // escreve GG
                }
            }
        }
    }

    fclose(f);
    return 1;
}

multi_level_operator_t *multi_level_operator_read(char *filename) {
    int i, j, k, type;
    char temp_name[1024];
    multi_level_operator_t *mop;
    multi_architecture_t *arch;
    window_t *joint;
    FILE *f;

    f = fopen(filename, "r");
    if (f == NULL) {
        return (multi_level_operator_t *) trios_error(MSG, "Failed to open %s.", filename);
    }
    fscanf(f, "%s", temp_name);
    if (strcmp(temp_name, "BB") == 0) {
        type = BB;
    } else if (strcmp(temp_name, "GG") == 0) {
        type = GG;
    } else {
        return (multi_level_operator_t *) trios_error(MSG, "Unknown operator type: %s.", temp_name);
    }

    arch = read_multi_architecture_data(f, filename);
    if (arch == NULL) {
        trios_error(MSG, "Invalid architecture file.");
        return NULL;
    }
    mop = multi_level_operator_create(arch, type);


    for (k = 0; k < mop->nlevels; k++) {
        for (i = 0; i < mop->levels[k].noperators; i++) {
            for (j = 0; j < mop->levels[k].ninputs; j++) {
                sprintf(temp_name, "%s-files/level%d/operator%d-files/operator", filename, k, i, j);
                if  (mop->type == BB) {
                    mop->levels[k].trained_operator[i] = (classifier_t *) itv_read(temp_name, &joint);
                }
            }
        }
    }
    multi_level_arch_free(arch);
    return mop;
}


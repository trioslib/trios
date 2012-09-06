#include "trios.h"

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
    return 0;
}

multi_level_operator_t *multi_level_operator_read(char *filename) {
    return 0;
}

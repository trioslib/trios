#include <trios_multi.h>
#include <stdlib.h>

multi_architecture_t *multi_level_arch_create(int nlevels, int *operators_in_level) {
    int i, j, k;
    multi_architecture_t *m;
    if (nlevels < 1) {
        return trios_error(MSG, "Cannot create operator with less than one level.");
    }
    for (i = 0; i < nlevels; i++) {
        if (operators_in_level[i] <= 0) {
            return trios_error(MSG, "Level %d has less than 1 operator.", operators_in_level[i]);
        }
    }
    if (operators_in_level[nlevels-1] != 1) {
        return trios_error(MSG, "Last level MUST have only one operator.");
    }
    m = malloc(sizeof(multi_architecture_t));

    m->nlevels = nlevels;
    m->levels = malloc(sizeof(multi_architecture_level_t) * nlevels);
    if (m->levels == NULL) {
        return trios_error(MSG, "Bad Alloc");
    }

    for (i = 0; i < nlevels; i++) {
        m->levels[i].ninputs = (i == 0)?1:m->levels[i-1].noperators;
        m->levels[i].noperators = operators_in_level[i];
        m->levels[i].windows = malloc(sizeof(window_t **) * m->levels[i].noperators);
        if (m->levels[i].windows == NULL) {
            return trios_error(MSG, "Bad Alloc");
        }
        for (j = 0; j < m->levels[i].noperators; j++) {
            m->levels[i].windows[j] = malloc(sizeof(window_t *) * m->levels[i].ninputs);
            if (m->levels[i].windows[j] == NULL) {
                return trios_error(MSG, "Bad Alloc");
            }
            for (k = 0; k < m->levels[i].ninputs; k++) {
                m->levels[i].windows[j][k] = NULL;
            }
        }
    }
    return m;
}

void multi_level_arch_free(multi_architecture_t *m) {
    int i, j, k;
    for (i = 0; i < m->nlevels; i++) {
        for (j = 0; j < m->levels[i].noperators; j++) {
            for (k = 0; k < m->levels[i].ninputs; k++) {
                win_free(m->levels[i].windows[j][k]);
            }
            free(m->levels[i].windows[j]);
        }
        free(m->levels[i].windows);
    }
    free(m->levels);
    free(m);
}


static void copy_window(window_t *w, window_t *window) {
    int i, j, k;
    for (i = 0; i < w->height; i++) {
        for (j = 0; j < w->width; j++) {
            for (k = 1; k <= w->nbands; k++) {
                int value = win_get_point(i, j, k, window);
                win_set_point(i, j, k, value, w);
            }
        }
    }
}

int multi_level_arch_set_window(multi_architecture_t *m, int level, int op, int input, window_t *window) {
    int i, j, k;
    if (m == NULL) {
        return trios_error(MSG, "Architecture pointer is null");
    }
    if (level >= m->nlevels) {
        return trios_error(MSG, "Operator has less than %d levels", level);
    }
    if (op >= m->levels[level].noperators) {
        return trios_error(MSG, "Level %d contains less than %d operators.", op);
    }
    if (input >= m->levels[level].ninputs) {
        return trios_error(MSG, "Operator %d in level %d contains less than %d inputs.", op, level, input);
    }
    m->levels[level].windows[op][input] = win_create(win_get_height(window), win_get_width(window), win_get_nbands(window));

    copy_window(m->levels[level].windows[op][input], window);
    return 1;
}


multi_level_operator_t *multi_level_operator_create(multi_architecture_t *m) {
    int i, j, k;
    multi_level_operator_t *op = malloc(sizeof(multi_level_operator_t));
    if (op == NULL) {
        return trios_error(MSG, "Bad alloc");
    }
    op->nlevels = m->nlevels;
    op->levels = malloc(sizeof(multi_level_operator_level_t) * op->nlevels);
    if (op->levels == NULL) {
        return trios_error(MSG, "Bad alloc");
    }
    for (i = 0; i < m->nlevels; i++) {
        op->levels[i].ninputs = m->levels[i].ninputs;
        op->levels[i].noperators = m->levels[i].noperators;

        op->levels[i].trained_operator = malloc(sizeof(itv_t) * op->levels[i].noperators);
        if (op->levels[i].trained_operator == NULL) {
            return trios_error(MSG, "Bad alloc");
        }

        op->levels[i].windows = malloc(sizeof(window_t **) * op->levels[i].noperators);
        if (op->levels[i].windows == NULL) {
            return trios_error(MSG, "Bad alloc");
        }
        for (j = 0; j < op->levels[i].noperators; j++) {
            op->levels[i].windows[j] = malloc(sizeof(window_t *) * op->levels[i].ninputs);
            if (op->levels[i].windows[j] == NULL) {
                return trios_error(MSG, "Bad alloc");
            }
            for (k = 0; k < op->levels[i].ninputs; k++) {
                window_t *w = &(m->levels[i].windows[j][k]);
                op->levels[i].windows[j][k] = win_create(win_get_height(w), win_get_width(w), win_get_nbands(w));
                 copy_window(&(op->levels[i].windows[j][k]), w);
            }
        }


    }
    return op;
}

void multi_level_operator_free(multi_level_operator_t *op) {
    int i, j;
    for (i = 0; i < op->nlevels; i++) {
        free(op->levels[i].trained_operator);
        for (j = 0; j < op->levels[i].noperators; j++) {
            free(op->levels[i].windows[j]);
        }
        free(op->levels[i].windows);
    }
    free(op->levels);
    free(op);
}

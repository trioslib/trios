#include <trios_multi.h>
#include <stdlib.h>

multi_architecture_t *multi_level_arch_create(int nlevels, int *operators_in_level) {
    int i, j;
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

    m->levels[0].ninputs = 1;
    m->levels[0].noperators = operators_in_level[0];
    for (i = 1; i < nlevels; i++) {
        m->levels[i].ninputs = m->levels[i-1].noperators;
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
        }
    }
    return m;
}

void multi_level_arch_free(multi_architecture_t *m) {
    int i, j, k;
    for (i = 0; i < m->nlevels; i++) {
        free(m->levels[i].windows);
    }
    free(m);
}


static void copy_window(window_t *w, window_t *window) {
    w->height = window->height;
    w->width = window->width;
    w->nbands = window->nbands;
    w->wsize = window->wsize;
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
    window_t *w = &(m->levels[level].windows[op][input]);
    copy_window(w, window);
    return 1;
}


multi_level_operator_t *multi_level_operator_create(multi_architecture_t *m) {

    return NULL;
}

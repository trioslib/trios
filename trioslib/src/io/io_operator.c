#include "trios.h"
#include <string.h>
#include "stdlib.h"

static int write_window(image_operator_t *iop, FILE *operator_file, char *path, int file_name_idx, char temp_string[]) {
    sprintf(temp_string, "%s-files/window", path);
    if (!win_write(temp_string, iop->win)) {
        return trios_error(MSG, "Error writing window at %s", temp_string);
    }
    sprintf(temp_string, "%s-files/window", path + file_name_idx);
    fprintf(operator_file, ".w\n%s\n", temp_string);
    return 1;
}

static int write_collec(image_operator_t *iop, FILE *operator_file, char *path, int file_name_idx, char temp_string[]) {
    if (iop->collec == NULL) return 1;
    sprintf(temp_string, "%s-files/collec", path);
    if (!xpl_write(temp_string, iop->collec, iop->win, iop->apt)) {
        return trios_error(MSG, "Error writing xpl at %s", temp_string);
    }
    sprintf(temp_string, "%s-files/collec", path + file_name_idx);
    fprintf(operator_file, ".xpl\n%s\n", temp_string);
    return 1;
}

static int write_decision(image_operator_t *iop, FILE *operator_file, char *path, int file_name_idx, char temp_string[]) {
    if (iop->decision == NULL) return 1;
    sprintf(temp_string, "%s-files/decision", path);
    if (!mtm_write(temp_string, iop->decision, iop->win, iop->apt)) {
        return trios_error(MSG, "Error writing mtm at %s", temp_string);
    }
    sprintf(temp_string, "%s-files/decison", path + file_name_idx);
    fprintf(operator_file, ".mtm\n%s\n", temp_string);
    return 1;
}

static int write_operator(image_operator_t *iop, FILE *operator_file, char *path, int file_name_idx, char temp_string[]) {
    if (iop->bb == NULL && iop->gg == NULL) return 1;
    sprintf(temp_string, "%s-files/operator", path);
    if (iop->type == BB) {
        if (!itv_write(temp_string, iop->bb, iop->win)) {
            return trios_error(MSG, "Error writing itv BB at %s", temp_string);
        }
        sprintf(temp_string, "%s-files/operator", path + file_name_idx);
        fprintf(operator_file, ".bb\n%s\n", temp_string);
    } else if (iop->type == GG) {
        if (!write_tree2(temp_string, iop->gg)) {
            return trios_error(MSG, "Error writing tree GG at %s", temp_string);
        }
        sprintf(temp_string, "%s-files/operator", path + file_name_idx);
        fprintf(operator_file, ".gg\n%s\n", temp_string);
    } else {

        return trios_error(MSG, "Operator not supported");
    }
    return 1;
}


int image_operator_write(char *path, image_operator_t *iop) {
    char temp_string[2048];
#ifdef WINDOWS
    char separator = '\\';
#else
    char separator = '/';
#endif
    int i, file_name_idx;
    FILE *operator_file = NULL;
    operator_file = fopen(path, "w");
    if (operator_file == NULL) {
        return trios_error(MSG, "Failed to open file %s\n", path);
    }
    if (iop->type == BB) {
        fprintf(operator_file, ".t\nBB\n");
    } else if (iop->type == GG) {
        fprintf(operator_file, ".t\nGG\n");
    }

    sprintf(temp_string, "mkdir %s-files", path);
    system(temp_string);

    for (i = strlen(path); i > 0; i--) {
        if (path[i-1] == separator) break;
    }
    file_name_idx = i;

    if (!write_window(iop, operator_file, path, file_name_idx, temp_string)) {
        return 0;
    }
    if (!write_collec(iop, operator_file, path, file_name_idx, temp_string)) {
        return 0;
    }
    if (!write_decision(iop, operator_file, path, file_name_idx, temp_string)) {
        return 0;
    }
    if (!write_operator(iop, operator_file, path, file_name_idx, temp_string)) {
        return 0;
    }
    return 1;
}

image_operator_t *image_operator_read(char *path) {
    image_operator_t *iop;
#ifdef WINDOWS
    char separator = '\\';
#else
    char separator = '/';
#endif


    trios_malloc(iop, sizeof(image_operator_t), image_operator_t *, "Failed to alloc image operator");
    iop->type = BB;
    iop->win = win;
    iop->apt = NULL;
    iop->gg = NULL;

    return iop;
}

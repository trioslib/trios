#include "trios.h"
#include "string.h"

void print_compacted_wpatt(FILE *dest, unsigned int *wpat, int wsize, int label) {
    int k, offset, bit;

    for (k = 0; k < wsize; k++) {
        bit = k % NB;
        offset = k / NB;
        fprintf(dest, "%d, ", (wpat[offset] & (1 << bit)) != 0 );
    }
    fprintf(dest, "%d\n", label);

}

void export_mtm_BB(mtm_t *mtm, FILE *dest) {
    int i, j;
    mtm_BX *bx = (mtm_BX *) mtm->mtm_data;

    for (i = 0; i < mtm->nmtm; i++) {
        for (j = 0; j < bx[i].fq; j++) {
            print_compacted_wpatt(dest, bx[i].wpat, mtm->wsize, bx[i].label);
        }
    }
}

void export_mtm(mtm_t *mtm, char *dest) {
    FILE *f;
    f = fopen(dest, "w");
    if (f == NULL) trios_fatal("Can't open file for writing.");

    if (mtm->type == BB) {
        export_mtm_BB(mtm, f);
    }

    fclose(f);
}

void export_xplBB_R(xpl_BB *xplBB, xpl_t *xpl, FILE *dest) {
    int i, k, offset, bit;
    unsigned int *data;

    if (xplBB == NULL) return;
    data = xplBB->wpat;

    for (i = 0; i < xplBB->fq0; i++) {
        print_compacted_wpatt(dest, data, xpl->wsize, 0);
    }

    for (i = 0; i < xplBB->fq1; i++) {
        print_compacted_wpatt(dest, data, xpl->wsize, 1);
    }

    export_xplBB_R(xplBB->left, xpl, dest);
    export_xplBB_R(xplBB->right, xpl, dest);
}

void export_xpl(xpl_t *xpl, char *dest) {
    FILE *f;
    f = fopen(dest, "w");
    if (f == NULL) trios_fatal("Can't open file for writing.");

    if (xpl->type == BB) {
        export_xplBB_R((xpl_BB *) (xpl->root), xpl, f);
    }


    fclose(f);
}

void print_usage() {
    printf("Usage: trios_csv [xpl|mtm] [BB] image_set window dest.csv\n");
    printf("\nThis tool exports the intermediary steps of the build process in the CSV format so they can be processed by other tools.\n");
    printf("\nThe xpl flag exports all window configurations and their respective labels.\n");
    printf("\nThe mtm flag exports all window configurations matched with the label that minimizes the MSE over the training set.\n");
}

int main(int argc, char *argv[]) {
    xpl_t *xpl;
    mtm_t *mtm;
    imgset_t *set;
    window_t *win;

    if (argc < 6) {
        print_usage();
        return 1;
    }

    set = imgset_read(argv[3]);
    if (set == NULL) {
        trios_fatal("Invalid image set.");
    }
    win = win_read(argv[4]);
    if (win == NULL) {
        trios_fatal("Invalid window file.");
    }


    if (strcmp("xpl", argv[1]) == 0) {
        xpl = lcollec_memory(set, win, BB);
        export_xpl(xpl, argv[5]);
        xpl_free(xpl);
    } else if (strcmp("mtm", argv[1]) == 0) {
        xpl = lcollec_memory(set, win, BB);
        mtm = ldecision_memory(xpl, 1, 0, AVERAGE, 0, 0);

        export_mtm(mtm, argv[5]);

        xpl_free(xpl);
        mtm_free(mtm);
    } else {
        print_usage();
        return 1;
    }

    win_free(win);
    imgset_free(set);

    return 0;
}

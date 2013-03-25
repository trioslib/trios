#include "trios.h"
#include "string.h"

void export_mtm_BB(mtm_t *mtm, FILE *dest) {
    int i, j, k, sf, bit;
    mtm_BX *bx = (mtm_BX *) mtm->mtm_data;

    for (i = 0; i < mtm->nmtm; i++) {
        for (j = 0; j < bx[i].fq + bx[i].fq1; j++) {
            for (k = 0; k < mtm->wsize; k++) {
                sf = k / NB;
                bit = k % NB;
                fprintf(dest, "%d ", (bx[i].wpat[sf] & (1 << bit) ) != 0);
            }
            fprintf(dest, "%d\n", bx[i].label);
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


void export_xpl(xpl_t *xpl) {


}

void export_xpl_R(xpl_t *xpl) {

}


int main(int argc, char *argv[]) {
    xpl_t *xpl;
    mtm_t *mtm;
    imgset_t *set;
    window_t *win;

    if (argc < 5) {
        printf("Usage: trios_csv [xpl|mtm] image_set window dest.csv");
        return 1;
    }

    set = imgset_read(argv[2]);
    if (set == NULL) {
        trios_fatal("Invalid image set.");
    }
    win = win_read(argv[3]);
    if (win == NULL) {
        trios_fatal("Invalid window file.");
    }


    if (strcmp("xpl", argv[1]) == 0) {
        xpl = lcollec_memory(set, win, BB);

        /* export csv */

        xpl_free(xpl);
    } else if (strcmp("mtm", argv[1]) == 0) {
        xpl = lcollec_memory(set, win, BB);
        mtm = ldecision_memory(xpl, 1, 0, AVERAGE, 0, 0);

        export_mtm(mtm, argv[4]);

        xpl_free(xpl);
        mtm_free(mtm);
    } else {
        printf("Usage: trios_csv [xpl|mtm] image_set window");
        return 1;
    }

    win_free(win);
    imgset_free(set);

    return 0;
}

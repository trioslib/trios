#include "trios_mtm.h"
#include "trios_win.h"
#include "trios_itv.h"
#include "stdlib.h"

itv_t *train_operator_ISI(unsigned int *data, int wsize, int npat, window_t *win) {
    int i, j, wzip;
    int shift, b;
    int label;
    itv_t *interval_list = NULL;
    unsigned int *zpat = NULL;
    
    mtm_t *decision = mtm_create(wsize, 0, npat);
    wzip = (wsize % NB == 0)? wsize / NB : wsize / NB + 1;
    zpat = (unsigned int *)malloc(sizeof(unsigned int) * wzip);
    for (i = 0; i < npat; i++) {
        for (j = 0; j < wzip; j++) {
            zpat[j] = data[i * (wzip+1) + j];
        }
        
        label = (data[i * (wzip+1) + wzip] >= 127)?255: 0;
        mtm_BX_insert(decision, i, wzip, zpat, label, label == 255, label == 255);
    }
    /* call isi partitioned !! */
    interval_list = lisi_partitioned(win, decision, 30000);
    mtm_free(decision);
    return interval_list;
}

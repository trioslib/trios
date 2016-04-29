#include "trios_mtm.h"
#include "trios_win.h"
#include "trios_itv.h"
#include "stdlib.h"

itv_t *train_operator_ISI(unsigned int *data, unsigned int *freqs, int wsize, int npat, window_t *win) {
    int i, j, wzip;
    int shift, b;
    int label, len;
    itv_t *interval_list = NULL;
    unsigned int *zpat = NULL;
    
    mtm_t *decision = mtm_create(wsize, 0, npat);
    unsigned int fq, fq1;
    decision->comp_prob = 1;
    wzip = (wsize % NB == 0)? wsize / NB : wsize / NB + 1;
    len = wzip+1;
    zpat = (unsigned int *)malloc(sizeof(unsigned int) * wzip);
    for (i = 0; i < npat; i++) {
        for (j = 0; j < wzip; j++) {
            zpat[j] = data[i * len + j];
        }
        
        label = data[i * len + wzip];
        fq = freqs[i * 2];
        fq1 = freqs[i*2+1];
        mtm_BX_insert(decision, i, wzip, zpat, label, fq, fq1);
    }
    mtm_write("teste22.mtm", decision, win);
    /* call isi partitioned !! */
    interval_list = lisi_partitioned(win, decision, 30000);
    mtm_free(decision);
    return interval_list;
}

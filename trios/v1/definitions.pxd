cimport cython
import cython

cdef extern from "trios_mtm.h":
    ctypedef struct mtm_t:
        int wsize
        char   type 
        unsigned int nmtm 
        unsigned int nsum 
        int    comp_prob 
        int   *mtm_data
    
    ctypedef struct mtm_BX:
        unsigned int   *wpat
        int            label
        unsigned int   fq 
        unsigned int   fq1


    mtm_t *mtm_create(int wsize, int type, unsigned int nmtm)
    
    void mtm_free(mtm_t *mtm)


# in itv_t, the extremes of the interval are bit-packed
cdef extern from "trios_itv.h":
    cdef struct itv_bx:
        unsigned int *A
        unsigned int *B 
        int label 
        itv_bx *next
    
    ctypedef itv_bx itv_BX
        
        
    
    ctypedef struct itv_t:
        int  wsize 
        int  wzip
        int  type
        int  nitv
        int  maxlabel
        int  deflabel       
        int  *head

cdef extern from "trios_win.h":
    ctypedef struct window_t:
        unsigned char width 
        unsigned char height 
        unsigned char nbands 
        int  *wsize 
        char *windata

    window_t *win_create(int height, int width, int nbands)
    int win_set_point(int i, int j, int k, int value, window_t *win)
    int win_get_point(int i, int j, int k, window_t *win)
    int win_free(window_t *win)

cdef extern window_t *win_read(char *fname)
cdef itv_t *itv_read(char *fname, window_t ** win)

cdef int itv_write(char *fname, itv_t *it, window_t *win)

cdef itv_t *train_operator_ISI(unsigned int *data, unsigned int *freqs, int wsize, int npat, window_t *win) nogil
cdef int itv_list_contain(itv_t * itv, unsigned int *wpat,	int wzip)

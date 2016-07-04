from cffi import FFI

c_code = FFI()

sources = ['trios/legacy/' + f for f in ['basic_win.c', 'basic_error.c', 'io_header.c', 'io_win.c', 'basic_itv.c', 'io_itv.c', 'basic_mtm.c', 'io_mtm.c', 'basic_common.c']]

sources.extend(['trios/legacy/ISI/' + f for f in ['dp_isi.c', 'ip_isi.c', 'ip_mincover.c', 'partition.c', 'train_operator.c']])


c_code.cdef('''
typedef struct {
  unsigned char width ;  /*!< window matrix width (1<=width<=256)  */
  unsigned char height ; /*!< window matrix height (1<=width<=256) */
  unsigned char nbands ; /*!< window matrix bands (1<=bands<=256)  */
  int  *wsize ;          /*!< window size                          */
  char *windata ;        /*!< window description array             */
} window_t ;

window_t *win_read(char *fname);
window_t *win_create(int height, int width, int nbands);
void win_free(window_t *);
int win_set_point(int, int, int, int, window_t *);
int win_get_point(int, int, int, window_t *);


typedef struct itv_bx { /*!< Structure of the interval                      +*/
  unsigned int *A ;     /*!< left extremity of the interval                 +*/
  unsigned int *B ;     /*!< right extremity of the interval                +*/
  int label ;           /*!< value associated to the points of the interval +*/
  struct itv_bx *next ; /*!< pointer to the next interval                   +*/
} itv_BX ;

typedef struct {
  int  wsize ;          /*!< w-pattern size +*/
  int  wzip ;           /*!< compacted w-pattern size +*/
  int  type ;           /*!< type (BB, BG, GB, GG) +*/
  int  nitv ;           /*!< number of intervals +*/
  int  maxlabel ;       /*!< maximum label assigned to the intervals in the set +*/
  int  deflabel ;       /*!< default value to be associated to the
                            points out of the intervals union +*/
  int  *head ;          /*!< pointer to the intervals list head +*/
} itv_t ;

itv_t *itv_read(char *fname, window_t ** win);
int itv_write(char *fname, itv_t * itv, window_t * win);

/* add stuff for isi here */
itv_t *train_operator_ISI(unsigned int *data, unsigned int *freqs, int wsize, int npat, window_t *win);
int itv_contain(itv_BX *p_itv, unsigned int *wpat, int wzip);
int itv_list_contain(itv_t *itv, unsigned int *wpat, int wzip);
''')


c_code.set_source('trios.legacy._legacy', '''
#include "trios_win.h"
#include "trios_itv.h"
#include "trios_error.h"
#include "trios_mtm.h"
#include "trios_misc.h"

#include "trios_learn.h"
#include "paclearn_local.h"

itv_t *train_operator_ISI(unsigned int *data, unsigned int *freqs, int wsize, int npat, window_t *win);


''', sources=sources, include_dirs=['trios/legacy/', 'trios/legacy/ISI/'])

if __name__ == '__main__':
    c_code.compile()

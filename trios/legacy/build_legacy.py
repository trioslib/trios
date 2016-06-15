from cffi import FFI

c_code = FFI()

sources = ['trios/legacy/' + f for f in ['basic_win.c', 'basic_error.c', 'io_header.c', 'io_win.c', 'basic_itv.c', 'io_itv.c', 'basic_mtm.c', 'basic_common.c']]

c_code.set_source('trios.legacy._legacy', '''
#include "trios_win.h"
#include "trios_itv.h"
#include "trios_error.h"
#include "trios_mtm.h"
#include "trios_misc.h"
''', sources=sources, include_dirs=['trios/legacy/'])

c_code.cdef('''
typedef struct {
  unsigned char width ;  /*!< window matrix width (1<=width<=256)  */
  unsigned char height ; /*!< window matrix height (1<=width<=256) */
  unsigned char nbands ; /*!< window matrix bands (1<=bands<=256)  */
  int  *wsize ;          /*!< window size                          */
  char *windata ;        /*!< window description array             */
} window_t ;

window_t *win_read(char *fname);
void win_free(window_t *);
int win_get_point(int, int, int, window_t *);

''')

if __name__ == '__main__':
    c_code.compile()
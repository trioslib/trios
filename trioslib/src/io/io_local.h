#ifndef _pacio_internals_h_
#define _pacio_internals_h_

#include "trios_apert.h"

apert_t *apert_read_data(FILE *fd);
void apert_write_data(FILE *fd, apert_t *apt);

#endif

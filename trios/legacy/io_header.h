#ifndef _io_header_h_
#define _io_header_h_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>

#define SIZECHAR 256

typedef struct Header_type {
  char fileType[9];		/* fileType[8] must be 0 */
  char rest[57];
} header_t;


/* ---------- io_header.c ---------------------------------------------- */

header_t         /*+ Purpose: read a header from a file                 +*/
  *header_read(
    FILE *fd     /*+ In: file descriptor                                +*/
    ) ;

void                /*+ Purpose: write a header into a file   +*/ 
  header_write(
    FILE *fd,         /*+ In: file descriptor                 +*/
    header_t *aHeader /*+ In: pointer to a Header structure   +*/
    ) ;

int                /*+ Purpose: compare two headers                       +*/
  header_compare(
    header_t *aHeader,    /*+ In: pointer to the fisrt header             +*/
    header_t *otherHeader /*+ In: pointer to the second header            +*/
    ) ;

int     /*+ Purpose: checks if the header keyword matches a given keyword +*/
  header_match(
    FILE     *fd,         /*+ In: File descriptor                         +*/
    const char     *keyword     /*+ In: a keyword (up to 8 characters)          +*/
    ) ;

#ifdef __cplusplus
}
#endif


#endif /* _io_header_h_ */

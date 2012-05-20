#include <pacbasic.h>
#include <pacio.h>
#include "pacio_local.h"

/* #define _DEBUG_ */


/*
     -------------------------------------------
     FUNCTION: header_read
     -------------------------------------------
*/

header_t         /*+ Purpose: read a header from a file                 +*/
  *header_read(
    FILE *fd     /*+ In: file descriptor                                +*/
  )
/*+ Return: a pointer to a Header structure on success, NULL on failure +*/
{

/* author Nina S. Tomita and R. Hirata Jr. (nina@ime.usp.br)             */
/* date: Mon Oct 21 1996                                                 */

  char      buffer[64];
  header_t  *aHeader;
  int	     index1=0;

  do buffer[index1++]= (char)getc(fd);
  while ( (buffer[index1-1] != EOF) && (index1 < 64) );

  if(index1 != 64)
    return (header_t *)pac_error(1, "header_read: file is too short.") ;

  if( !(aHeader= (header_t *)malloc(sizeof(header_t))) ) {
    return (header_t *)pac_error(1, "header_read: memory alocation error.");
  }
  strncpy(aHeader->fileType, buffer, 8);
  aHeader->fileType[8]= 0;
  strncpy(aHeader->rest, &buffer[8], 56);

  return(aHeader);
}


/*
     -------------------------------------------
     FUNCTION: header_write
     -------------------------------------------
*/

void                /*+ Purpose: write a header into a file   +*/ 
  header_write(
    FILE *fd,         /*+ In: file descriptor                 +*/
    header_t *aHeader /*+ In: pointer to a Header structure   +*/
  )
/*+ Return nothing                                            +*/
{

/* author: Nina S. Tomita and R. Hirata Jr.(nina@ime.usp.br)   */
/* date: Mon Oct 21 1996                                       */

  int i ;
  int aux ;
  
  aux = strlen(aHeader->fileType);
  if (aux < 8) {
    for (i=aux; i<8; i++) aHeader->fileType[i] = ' ';
  }
  aHeader->fileType[8] = '\0';
  
  fprintf(fd, "%s", aHeader->fileType) ;

  aux = strlen(aHeader->rest);
  if (aux < 56) {
    for (i=aux; i<56; i++) aHeader->rest[i] = '#';
  }

  fprintf(fd, "%s\n", aHeader->rest) ;

}


/*
     -------------------------------------------
     FUNCTION: header_compare
     -------------------------------------------
*/

int                /*+ Purpose: compare two headers                       +*/
  header_compare(
    header_t *aHeader,    /*+ In: pointer to the first header             +*/
    header_t *otherHeader /*+ In: pointer to the second header            +*/
  )
/*+ Return: 1 if they are equal, 0 if they are different                  +*/
{

/* author: Nina S. Tomita and R. Hirata Jr. (nina@ime.usp.br)              */
/* date: Mon Oct 21 1996                                                   */

  return(!strcmp(aHeader->fileType, otherHeader->fileType));

}



/*
     -------------------------------------------
     FUNCTION: header_match
     -------------------------------------------
*/

int     /*+ Purpose: checks if the header keyword matches a given keyword +*/
  header_match(
    FILE     *fd,         /*+ In: File descriptor                         +*/
    const char     *keyword     /*+ In: a keyword (up to 8 characters)          +*/
  )
/*+ Return: 1 if they match, 0 if they don't match                        +*/
{

/* author: Nina S. Tomita and R. Hirata Jr. (nina@ime.usp.br)              */
/* date: Thu Feb 17 2000                                                   */

  header_t *aHeader ;
  int      result ;

  /* read file header */
  aHeader= header_read(fd);  
  if(!aHeader) {
    return /*(apert_t *)*/pac_error(1, "header_match: header_head() failed.") ;
  }
  
#ifdef _DEBUG_ 
  pac_debug("header_read() done") ;  
#endif

  result = !((int)strcmp(aHeader->fileType, keyword)) ;
  free( aHeader ) ;

  return( result ) ;
}

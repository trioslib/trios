#include <pacbasic.h>


/*
     -------------------------------------------
     FUNCTION: pac_error
     -------------------------------------------
*/

int        /*+ Purpose: Prints the error messages of the pac routines      +*/
  pac_error( 
    int type,              /*+ In: A flag
                                   1 : print message
                                   0 : do not print message                +*/
    const char* format,          /*+ In: The error message and the arguments     +*/
  ... )
/*+ Return: ZERO                                                           +*/
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                  */
/* date: Fri Nov 29 1996                                                    */

   va_list     args;

  if (type == 1) {
    fprintf(stderr, "\n *** ERROR: ") ;
    va_start( args, format );
    vfprintf( stderr, format, args );
    fprintf(stderr, "\n\n");
    va_end( args );
  }
  else if(type == 2) {
    va_start( args, format );
    vfprintf( stderr, format, args );
    fputc( '\n', stderr);
    va_end( args );
  }

  return 0;
}



/*
     -------------------------------------------
     FUNCTION: pac_fatal
     -------------------------------------------
*/

void       /*+ Purpose: Prints the error messages of the pac routines     +*/
  pac_fatal( 
    const char* format,       /*+ In: The error message and the arguments       +*/
  ... )
/*+ Return: nothing                                                       +*/ 
{
/* author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/* date: Fri Nov 29 1996                                                   */

  va_list     args;

  fprintf(stderr, "\n *** FATAL ERROR: ") ;
  va_start( args, format );
  vfprintf( stderr, format, args );
  fprintf(stderr, "\n\n");
  va_end( args );

  exit(1);
}


/*
     -------------------------------------------
     FUNCTION: pac_debug
     -------------------------------------------
*/
 
void            /*+ Purpose: print message for debugging purposes         +*/
  pac_debug(                                                                  
    const char* format,       /*+ In: The error message and the arguments       +*/
  ... )
/*+ Return: nothing                                                       +*/
{
 
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                */
/*  date: Mon Mar 30 1998                                                  */

  va_list     args;

  fprintf(stderr, "(debug) ") ;
  va_start( args, format );
  vfprintf( stderr, format, args );
  fputc( '\n', stderr);
  va_end( args );
}

/*
     -------------------------------------------
     FUNCTION: pac_warning
     -------------------------------------------
*/
 
void            /*+ Purpose: print message for warning purposes           +*/
  pac_warning(                                                                  
    const char* format,       /*+ In: The warning message and the arguments     +*/
  ... )
/*+ Return: nothing                                                       +*/
{
 
/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                */
/*  date: Mon Mar 30 1998                                                  */

  va_list     args;

  fprintf(stderr, "\n *** WARNING: ") ;
  va_start( args, format );
  vfprintf( stderr, format, args );
  fprintf(stderr, "\n");
  va_end( args );
}





#include <stdarg.h>
#include <stdio.h>

/*!
  Prints error messages. Works like printf.

  \param type A flag 1 : print message; 0 : do not print message.
  \param format The error message and the arguments.
  \param va_list Arguments to the error message (like printf).
  \return Always return 0.
*/
int trios_error(int type, const char* format, ...) {
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

/*!
  Prints fatal error messsages and exit. Works like printf.

  \param type A flag 1 : print message; 0 : do not print message.
  \param format The error message and the arguments.
  \param va_list Arguments to the error message (like printf).
*/
void trios_fatal(const char* format, ...) {
  va_list     args;

  fprintf(stderr, "\n *** FATAL ERROR: ") ;
  va_start( args, format );
  vfprintf( stderr, format, args );
  fprintf(stderr, "\n\n");
  va_end( args );

  exit(1);
}


/*!
  Prints debug messages. Works like printf.

  \param type A flag 1 : print message; 0 : do not print message.
  \param format The error message and the arguments.
  \param va_list Arguments to the error message (like printf).
*/
void trios_debug(const char* format, ...) {
  va_list     args;

  fprintf(stderr, "(debug) ") ;
  va_start( args, format );
  vfprintf( stderr, format, args );
  fputc( '\n', stderr);
  va_end( args );
}

/*!
  Prints warning messages. Works like printf.

  \param type A flag 1 : print message; 0 : do not print message.
  \param format The error message and the arguments.
  \param va_list Arguments to the error message (like printf).
*/
void trios_warning(const char* format, ...) {
  va_list     args;

  fprintf(stderr, "\n *** WARNING: ") ;
  va_start( args, format );
  vfprintf( stderr, format, args );
  fprintf(stderr, "\n");
  va_end( args );
}





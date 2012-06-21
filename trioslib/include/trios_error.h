#ifndef _trios_error_h_
#define _trios_error_h_


/*!
  Prints error messages. Works like printf.

  \param type A flag 1 : print message; 0 : do not print message.
  \param format The error message and the arguments.
  \param va_list Arguments to the error message (like printf).
  \return Always return 0.
*/
int trios_error(int type, const char* format, ...);

/*!
  Prints fatal error messsages and exit. Works like printf.

  \param type A flag 1 : print message; 0 : do not print message.
  \param format The error message and the arguments.
  \param va_list Arguments to the error message (like printf).
*/
void trios_fatal(const char* format, ...);

/*!
  Prints debug messages. Works like printf.

  \param type A flag 1 : print message; 0 : do not print message.
  \param format The error message and the arguments.
  \param va_list Arguments to the error message (like printf).
*/
void trios_debug(const char* format, ...);

#endif

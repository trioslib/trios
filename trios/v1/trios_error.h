#ifndef _trios_error_h_
#define _trios_error_h_

#ifdef __cplusplus
extern "C" {
#endif


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

/*!
  Prints warning messages. Works like printf.

  \param type A flag 1 : print message; 0 : do not print message.
  \param format The error message and the arguments.
  \param va_list Arguments to the error message (like printf).
*/
void trios_warning(const char* format, ...);

/*!
 * Macro that expands to a malloc with error checking and on error behavior.
 */
#define trios_malloc(var_name, sz, ret_type, error_msg, ...) \
    var_name = malloc(sz);  \
    if (var_name == NULL) { \
        return (ret_type) trios_error(MSG, error_msg, ##__VA_ARGS__); \
    } \



#ifdef __cplusplus
}
#endif


#endif

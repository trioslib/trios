#ifndef _trios_appl_h_
#define _trios_appl_h_

#ifdef __cplusplus
extern "C" {
#endif


/*!
    Applies an operator in an image.

    \param f_appl Input image filename.
    \param f_basis Basis file.
    \param cv
    \param hash_flag Switch between normal and hash application.
    \param on_value Value of the output pixels.
    \param f_out Output image filename.
    \return 1 on success. 0 on failure.
 */
int  lpapplic(char *f_appl, char *f_basis, char *f_mask,  int  cv, int  hash_flag, int  on_value, char *f_out );

#ifdef __cplusplus
}
#endif


#endif


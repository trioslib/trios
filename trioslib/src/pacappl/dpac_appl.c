#include <paclib.h>

#include <pac_img.h>

int  lpapplic(char *f_appl, char *f_basis, char *f_mask,  int  cv, int  hash_flag, int  on_value, char *f_out ) {
  int     width, height, npixels , type ;

  img_t *img_appl, /* In: Image to be operated                           */
          *img_mask, /* In: Mask image                                     */
          *img_out ; /* Out: resulting output image                        */ 
  img_t *img_tmp ; /* temporary image */
  unsigned short *uspixels ;  /*  Image to be operated                */
  unsigned char  *ucpixels1 ; /*  Mask image                          */
  unsigned char  *ucpixels2 ; /*  resulting output image (byte)       */
  unsigned short *uspixels2 ; /*  resulting output image (short)      */
  window_t *win ;
  apert_t  *apt ;
  itv_t    *itv ;
  /* read input image file */
  img_appl = img_readPGM(f_appl);
  if(!img_appl) {
    return pac_error(MSG, "lpapplic: img_readPGM() failed.") ;
  }
  width = img_get_width(img_appl) ;
  height = img_get_height(img_appl);
  if(mm_pixeldatatype(img_appl) != MM_USHORT) {
    img_tmp = mm_createimage(width, height, 1, MM_USHORT) ;
    if(!img_tmp) {
      img_free(img_appl) ;
      return pac_error(MSG, "lpapplic: mm_createimage() failed.") ;
    }
    if(!mm_conv(img_appl, img_tmp, MM_USHORT)) {
      return pac_error(MSG, "lpapplic: mm_conv() failed.") ;
    }
    img_free(img_appl) ;
    img_appl = img_tmp ;
  }
  if ((itv = itv_read(f_basis, &win, &apt))==NULL) {
    img_free(img_appl) ;
    return pac_error(MSG, "lpapplic: itv_read() failed.") ;
  }
  /* read or create and set mask image border to Zero */
  if(!(img_mask = set_mask(f_mask, width, height, win))) {
    img_free(img_appl) ;
    return pac_error(MSG, "lpapplic: set_mask() failed.") ;    
  }
  uspixels = (unsigned short *)mm_pixels(img_appl) ;
  ucpixels1 = (unsigned char *)mm_pixels(img_mask) ;
  /* creates output image : SHORT if type==BG and maximum label > 255 */
  npixels = mm_npixels(img_appl) ;
  type = itv_get_type(itv) ;

  img_out = NULL ;
  ucpixels2 = NULL ;
  uspixels2 = NULL ; 
  if(type==BG) {
   if(itv_get_maxlabel(itv) > 255) {
      img_out = mm_createimage(width, height, 1, MM_USHORT) ;
      if(!(img_out)) {
        img_free(img_appl) ;
        img_free(img_mask) ;
        return pac_error(MSG, "lpapplic: mm_createimage() failed.") ;
      }
      mm_const_image(img_out, (double)0) ;  
      uspixels2 = (unsigned short *)mm_pixels(img_out) ;
    }
    else {
      img_out = mm_createimage(width, height, 1, MM_UBYTE) ;
      if(!(img_out)) {
        img_free(img_appl) ;
        img_free(img_mask) ;
        return pac_error(MSG, "lpapplic: mm_createimage() failed.") ;
      }
      mm_const_image(img_out, (double)0) ;  
      ucpixels2 = (unsigned char *)mm_pixels(img_out) ;
    }
  }
  else {
    img_out = mm_createimage(width, height, 1, MM_UBYTE) ;
    if(!(img_out)) {
      img_free(img_appl) ;
      img_free(img_mask) ;
      return pac_error(MSG, "lpapplic: mm_createimage() failed.") ;
    }
    mm_const_image(img_out, (double)0) ;  
    ucpixels2 = (unsigned char *)mm_pixels(img_out) ;
    /* Change label of the intervals in the basis by "on_value" */
    if(!itv_label(itv, 1, on_value)) {
      img_free(img_appl) ;
      img_free(img_mask) ;
      return pac_error(MSG, "lpapplic: itv_label() failed.") ;
    }

  }
  switch(type) {
    case BB:
    case BG:
      if(itv_get_maxlabel(itv) <= 255 ) {       
        /* if the window size is too large, probably there will be 
	   few repetitions of the patterns                          */
        if ((hash_flag==0) || (win_get_wsize(win) > 32)) { 
          if(!papplic_bx(uspixels, ucpixels1, ucpixels2, cv,
                         itv, win, width, npixels)) {  
        img_free(img_appl) ;
        img_free(img_mask) ;
            img_free(img_out) ;
	    pac_error(MSG, "lpapplic : pacapplic_bx() failed.") ;
	  }
        }
        else {
	  if(!hashapplic_bx(uspixels, ucpixels1, ucpixels2, cv,
                            itv, win, width, npixels)) { 
        img_free(img_appl) ;
        img_free(img_mask) ;
        img_free(img_out) ;
	    pac_error(MSG, "lpapplic : hashapplic_bx() failed.") ;
  	  }
        }
      }
      else {
        if ((hash_flag==0) || (win_get_wsize(win) > 32)) { 
          if(!papplic_short(uspixels, ucpixels1, uspixels2, cv,
                            itv, win, width, npixels)) {  
        img_free(img_appl) ;
        img_free(img_mask) ;
            img_free(img_out) ;
	    pac_error(MSG, "lpapplic : pacapplic_short() failed.") ;
	  }
        }
        else {
	  if(!hashapplic_short(uspixels, ucpixels1, uspixels2, cv,
                               itv, win, width, npixels)) { 
        img_free(img_appl) ;
        img_free(img_mask) ;
        img_free(img_out) ;
	    pac_error(MSG, "lpapplic : hashapplic_short() failed.") ;
  	  }
        }
      }
      break ;
    default: pac_error(1, "Types GG or WK. See routine lapplicDT().") ;
  }
  if(!mm_writeimage(f_out, img_out)) {
    img_free(img_appl) ;
    img_free(img_mask) ;
    img_free(img_out) ;
    pac_error(MSG, "lpapplic : mm_writeimage() failed.") ;
  }

  itv_free(itv) ;
  win_free(win) ;

  img_free(img_appl) ;
  img_free(img_mask) ;
  img_free(img_out) ;

  return(1) ;

}








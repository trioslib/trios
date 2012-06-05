SET (PAC_HEADERS pacbasic.h
                 pacio.h
                 pacapplic.h
                 pac_apert.h
                 pacappl.h
                 pac_error.h
                 pac_imgset.h
                 pac_img.h
                 pac_itv.h
                 pac_kernel.h
                 pac_mtm.h
                 pac_win.h
                 pac_xpl.h
                 xpl_collec.h
                 )

INSTALL(FILES ${PAC_HEADERS}  DESTINATION include)

SET (PAC_HEADERS pacbasic.h
                 pacio.h
                 pacappl.h
                 pac_imgset.h

                 pac_itv.h
                 pac_mtm.h

                 pac_xpl.h
                 xpl_collec.h
                 trios.h
                 trios_win.h
                 trios_img.h
                 trios_error.h
                 trios_appl.h
                 )

INSTALL(FILES ${PAC_HEADERS}  DESTINATION include)

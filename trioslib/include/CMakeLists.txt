SET(TRIOS_HEADERS pacbasic.h


                 pac_itv.h
                 pac_mtm.h

                 pac_xpl.h
                 xpl_collec.h

                 trios.h
                 trios_appl.h
                 trios_win.h
                 trios_img.h
                 trios_error.h
                 trios_io.h
                 trios_imgset.h
                 )

INSTALL(FILES ${TRIOS_HEADERS}  DESTINATION ${TRIOS_BINARY_DIR}/include)
file(COPY ${TRIOS_HEADERS} DESTINATION  ${CMAKE_BINARY_DIR}/include)

SET(TRIOS_HEADERS pacbasic.h
                 pac_kernel.h
                 pac_xpl.h
                 xpl_collec.h

                 trios.h
                 trios_appl.h
                 trios_win.h
                 trios_img.h
                 trios_error.h
                 trios_io.h
                 trios_imgset.h
                 trios_mtm.h
                 trios_itv.h
                 )

INSTALL(FILES ${TRIOS_HEADERS}  DESTINATION ${TRIOS_BINARY_DIR}/include)
file(COPY ${TRIOS_HEADERS} DESTINATION  ${CMAKE_BINARY_DIR}/include)

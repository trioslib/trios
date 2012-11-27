SET(TRIOS_HEADERS
                 trios.h
                 trios_appl.h
                 trios_win.h
                 trios_img.h
                 trios_error.h
                 trios_io.h
                 trios_imgset.h
                 trios_mtm.h
                 trios_itv.h
                 io_header.h
                 trios_misc.h
                 trios_learn.h
                 trios_collec.h
                 trios_xpl.h
                 trios_common.h
                 trios_multi.h
                 trios_operator.h
                 trios_apert.h
                 )

INSTALL(FILES ${TRIOS_HEADERS}  DESTINATION ${TRIOS_BINARY_DIR}/include)
file(COPY ${TRIOS_HEADERS} DESTINATION  ${CMAKE_BINARY_DIR}/include)

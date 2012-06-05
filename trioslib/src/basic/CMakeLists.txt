
PROJECT(TRIOS_BASIC)
SET(CMAKE_BUILD_TYPE debug)
SET(TRIOS_BASIC_SRCS
basic_cmp.c
basic_common.c
basic_error.c
basic_imgset.c
basic_img.c
basic_itv.c
basic_mtm.c
basic_win.c
basic_xpl.c
basic_xplBB.c)
ADD_LIBRARY(triosbasic ${TRIOS_BASIC_SRCS})

INSTALL(TARGETS triosbasic
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

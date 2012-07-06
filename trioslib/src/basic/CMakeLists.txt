
PROJECT(TRIOS_BASIC)
SET(CMAKE_BUILD_TYPE debug)
SET(TRIOS_BASIC_SRCS
basic/basic_cmp.c
basic/basic_common.c
basic/basic_error.c
basic/basic_imgset.c
basic/basic_img.c
basic/basic_itv.c
basic/basic_mtm.c
basic/basic_multi.c
basic/basic_win.c
basic/basic_xpl.c
basic/basic_xplBB.c)
ADD_LIBRARY(triosbasic ${TRIOS_BASIC_SRCS})

INSTALL(TARGETS triosbasic
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

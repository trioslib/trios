
PROJECT(TRIOS_BASIC)
SET(CMAKE_BUILD_TYPE debug)
SET(TRIOS_BASIC_SRCS
pac_cmp.c 
pac_common.c 
pac_error.c 
pac_imgset.c 
pac_img.c
pac_itv.c
pac_mtm.c
pac_win.c 
pac_xpl.c 
pac_xplBB.c)
ADD_LIBRARY(triosbasic ${TRIOS_BASIC_SRCS})

INSTALL(TARGETS triosbasic
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

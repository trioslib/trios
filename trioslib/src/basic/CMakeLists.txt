
PROJECT(TRIOS_BASIC)
SET(CMAKE_BUILD_TYPE debug)
SET(TRIOS_BASIC_SRCS
cmp.c
common.c
error.c
imgset.c
img.c
itv.c
mtm.c
win.c
xpl.c
xplBB.c)
ADD_LIBRARY(triosbasic ${TRIOS_BASIC_SRCS})

INSTALL(TARGETS triosbasic
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

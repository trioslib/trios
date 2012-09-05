
PROJECT(TRIOS_APPL)
SET(CMAKE_BUILD_TYPE debug)
SET(TRIOS_APPL_SRCS
appl/appl.c
appl/appl_bb.c
appl/appl_gg.c
appl/appl_wk.c
appl/appl_multi.c
)
ADD_LIBRARY(triosappl ${TRIOS_APPL_SRCS})

INSTALL(TARGETS triosappl
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)


PROJECT(TRIOS_APPL)
SET(CMAKE_BUILD_TYPE debug)
SET(TRIOS_APPL_SRCS
appl/appl.c
appl/appl_bb.c
appl/appl_gg.c
appl/appl_wk.c
appl/appl_multi.c
appl/operator.c
)
ADD_LIBRARY(triosappl ${TRIOS_COLLEC_SRCS} ${TRIOS_APPL_SRCS})

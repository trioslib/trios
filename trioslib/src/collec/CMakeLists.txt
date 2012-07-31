PROJECT(TRIOS_COLLEC)

set(TRIOS_COLLEC_SRCS
    collec/collec_xpl.c
    collec/collec_xpl_bb.c
    collec/collec_xpl_gg.c
    collec/collec_xpl_threshold.c
    collec/collec_xpl_multi_level.c
)

add_library(trios_collec ${TRIOS_COLLEC_SRCS})
target_link_libraries(trios_collec triosbasic triosio m)


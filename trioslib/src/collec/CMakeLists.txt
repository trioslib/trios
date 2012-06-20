PROJECT(TRIOS_COLLEC)
add_library(trios_collec collec_xpl.c collec_xpl_threshold.c)
target_link_libraries(trios_collec triosbasic triosio m)


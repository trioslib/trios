PROJECT(TRIOS_COLLEC)
add_library(trios_collec trios_xpl_collec.c)
target_link_libraries(trios_collec pacbasic pacio m)


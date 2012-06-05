PROJECT(TRIOS_COLLEC)
add_library(trios_collec collec_xpl.c)
target_link_libraries(trios_collec pacbasic pacio m)


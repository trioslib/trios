
add_executable(trios_build trios_build.c)
target_link_libraries(trios_build trios m)

add_executable(trios_apply trios_apply.c)
target_link_libraries(trios_apply trios m)

add_executable(trios_test trios_test.c)
target_link_libraries(trios_test trios m)

add_executable(trios_csv trios_csv.c)
target_link_libraries(trios_csv trios m)

add_executable(trios_collec trios_collec.c)
target_link_libraries(trios_collec trios m)

add_executable(trios_relief trios_relief.c)
target_link_libraries(trios_relief trios m)

add_executable(trios_xpl_util trios_xpl_util.c)
target_link_libraries(trios_xpl_util trios m)

add_executable(trios_win_select trios_win_select.c)
target_link_libraries(trios_win_select trios m)

install(TARGETS trios_csv trios_test trios_apply trios_build
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION bin)

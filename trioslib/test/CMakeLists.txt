SET(CMAKE_BUILD_TYPE debug)

add_executable(pac_win_test pac_win_test.c)
target_link_libraries(pac_win_test pacio pacbasic m)

add_executable(pac_xpl_test pac_xpl_test.c)
target_link_libraries(pac_xpl_test pacio pacbasic m)

add_executable(pac_img_test pac_img_test.c)
target_link_libraries(pac_img_test pacio pacbasic m)

add_executable(trios_collec_test trios_collec_test.c)
target_link_libraries(trios_collec_test pacio pacbasic trios_collec m)

add_executable(trios_decision_test trios_decision_test.c)
target_link_libraries(trios_decision_test pacio pacbasic trios_collec paclearn m)

add_executable(trios_isi_test trios_isi_test.c)
target_link_libraries(trios_isi_test pacio pacbasic trios_collec paclearn m)

add_executable(trios_group_test trios_group_test.c)
target_link_libraries(trios_group_test pacio pacbasic trios_collec paclearn m)

add_executable(trios_applic_test trios_applic_test.c)
target_link_libraries(trios_applic_test pacio pacbasic trios_collec pacappl paclearn m)


file(COPY test_read_win.txt DESTINATION  ${CMAKE_BINARY_DIR}/bin)
file(COPY test_img DESTINATION  ${CMAKE_BINARY_DIR}/bin)

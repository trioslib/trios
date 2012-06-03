SET(CMAKE_BUILD_TYPE debug)

add_executable(trios_win_test trios_win_test.c)
target_link_libraries(trios_win_test triosio triosbasic m)

add_executable(trios_xpl_test trios_xpl_test.c)
target_link_libraries(trios_xpl_test triosio triosbasic m)

add_executable(trios_img_test trios_img_test.c)
target_link_libraries(trios_img_test triosio triosbasic m)

add_executable(trios_collec_test trios_collec_test.c)
target_link_libraries(trios_collec_test triosio triosbasic trios_collec m)

add_executable(trios_decision_test trios_decision_test.c)
target_link_libraries(trios_decision_test triosio triosbasic trios_collec trioslearn m)

add_executable(trios_isi_test trios_isi_test.c)
target_link_libraries(trios_isi_test triosio triosbasic trios_collec trioslearn m)

add_executable(trios_group_test trios_group_test.c)
target_link_libraries(trios_group_test triosio triosbasic trios_collec trioslearn m)

add_executable(trios_applic_test trios_applic_test.c)
target_link_libraries(trios_applic_test triosio triosbasic trios_collec triosappl trioslearn m)


file(COPY test_read_win.txt DESTINATION  ${CMAKE_BINARY_DIR}/bin)
file(COPY test_img DESTINATION  ${CMAKE_BINARY_DIR}/bin)

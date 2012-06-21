SET(CMAKE_BUILD_TYPE debug)

add_executable(trios_win_test trios_win_test.c)
target_link_libraries(trios_win_test triosio triosbasic m)
add_test(WinTest ${TRIOS_BINARY_DIR}/bin/trios_win_test)

add_executable(trios_xpl_test trios_xpl_test.c)
target_link_libraries(trios_xpl_test triosio triosbasic m)
add_test(XplTest ${TRIOS_BINARY_DIR}/bin/trios_xpl_test)

add_executable(trios_img_test trios_img_test.c)
target_link_libraries(trios_img_test triosio triosbasic m)
add_test(NAME ImgTest WORKING_DIRECTORY ${TRIOS_BINARY_DIR}/bin/ COMMAND ${TRIOS_BINARY_DIR}/bin/trios_img_test )

add_executable(trios_collec_test trios_collec_test.c)
target_link_libraries(trios_collec_test triosio triosbasic trios_collec m)
add_test(NAME CollecTest WORKING_DIRECTORY ${TRIOS_BINARY_DIR}/bin/ COMMAND ${TRIOS_BINARY_DIR}/bin/trios_collec_test )

add_executable(trios_collec_threshold_test trios_collec_threshold_test.c)
target_link_libraries(trios_collec_threshold_test triosio triosbasic trios_collec m)
add_test(NAME CollecThresholdTest WORKING_DIRECTORY ${TRIOS_BINARY_DIR}/bin/ COMMAND ${TRIOS_BINARY_DIR}/bin/trios_collec_threshold_test )


add_executable(trios_decision_test trios_decision_test.c)
target_link_libraries(trios_decision_test triosio triosbasic trios_collec trioslearn m)
add_test(NAME DecisionTest WORKING_DIRECTORY ${TRIOS_BINARY_DIR}/bin/ COMMAND ${TRIOS_BINARY_DIR}/bin/trios_decision_test )

add_executable(trios_isi_test trios_isi_test.c)
target_link_libraries(trios_isi_test triosio triosbasic trios_collec trioslearn m)
add_test(NAME ISITest WORKING_DIRECTORY ${TRIOS_BINARY_DIR}/bin/ COMMAND ${TRIOS_BINARY_DIR}/bin/trios_isi_test )

add_executable(trios_applic_test trios_applic_test.c)
target_link_libraries(trios_applic_test triosio triosbasic trios_collec triosappl trioslearn m)
add_test(NAME ApplicTest WORKING_DIRECTORY ${TRIOS_BINARY_DIR}/bin/ COMMAND ${TRIOS_BINARY_DIR}/bin/trios_applic_test )


file(COPY test_read_win.txt DESTINATION  ${CMAKE_BINARY_DIR}/bin)
file(COPY test_img DESTINATION  ${CMAKE_BINARY_DIR}/bin)

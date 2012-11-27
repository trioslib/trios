
add_executable(trios_build trios_build.c)
target_link_libraries(trios_build trios m)

add_executable(trios_apply trios_apply.c)
target_link_libraries(trios_apply trios m)

add_executable(trios_test trios_test.c)
target_link_libraries(trios_test trios m)


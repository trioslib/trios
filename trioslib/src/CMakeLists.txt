
include(basic/CMakeLists.txt)
include(appl/CMakeLists.txt)
include(learn/CMakeLists.txt)
include(collec/CMakeLists.txt)
include(io/CMakeLists.txt)
include(opencv_interface/CMakeLists.txt)
include(window_selection/CMakeLists.txt)

add_library(trios STATIC ${TRIOS_BASIC_SRCS} ${TRIOS_COLLEC_SRCS} ${TRIOS_APPL_SRCS}  ${TRIOS_LEARN_SRCS} ${TRIOS_IO_SRCS} ${TRIOS_CV_SRCS} ${TRIOS_FS_SRCS})
target_link_libraries(trios -fopenmp opencv_core opencv_ml )

add_library(trios_shared SHARED ${TRIOS_BASIC_SRCS} ${TRIOS_COLLEC_SRCS} ${TRIOS_APPL_SRCS}  ${TRIOS_LEARN_SRCS} ${TRIOS_IO_SRCS} ${TRIOS_CV_SRCS} ${TRIOS_FS_SRCS})
target_link_libraries(trios -fopenmp opencv_core opencv_ml )

set_target_properties(trios_shared PROPERTIES OUTPUT_NAME trios)

install(TARGETS trios
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(TARGETS trios_shared
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

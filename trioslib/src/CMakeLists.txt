
include(basic/CMakeLists.txt)
include(appl/CMakeLists.txt)
include(learn/CMakeLists.txt)
include(collec/CMakeLists.txt)
include(io/CMakeLists.txt)
include(opencv_interface/CMakeLists.txt)

add_library(trios ${TRIOS_BASIC_SRCS} ${TRIOS_COLLEC_SRCS} ${TRIOS_APPL_SRCS}  ${TRIOS_LEARN_SRCS} ${TRIOS_IO_SRCS} ${TRIOS_CV_SRCS})
target_link_libraries(trios ${OpenCV_LIBS} ${OpenMP_LIBS})

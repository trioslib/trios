
SET(TRIOS_CV_SRCS
opencv_interface/apply.cpp
opencv_interface/training.cpp
opencv_interface/classifier_io.cpp
)

ADD_LIBRARY(trioscv ${TRIOS_CV_SRCS})
target_link_libraries(trioslearn ${OpenCV_LIBS} m)


INSTALL(TARGETS trioscv
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

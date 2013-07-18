PROJECT(TRIOS_CV)
SET(TRIOS_CV_SRCS

opencv_interface/apply.cpp
opencv_interface/training.cpp
opencv_interface/classifier_io.cpp
)

ADD_LIBRARY(trioscv ${TRIOS_CV_SRCS})
target_link_libraries(trioscv opencv_core opencv_ml triosbasic trios_collec m)


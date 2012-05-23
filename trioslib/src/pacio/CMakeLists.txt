PROJECT(PACIO)
SET(PACIO_SRCS 
io_header.c			   
io_imgset.c
io_img.c
io_mtm.c
io_showxpl.c			  
io_win.c
io_xpl.c
)
ADD_LIBRARY(pacio ${PACIO_SRCS})

INSTALL(TARGETS pacio
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

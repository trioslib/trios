PROJECT(TRIOS_IO)
SET(TRIOS_IO_SRCS
io_header.c			   
io_imgset.c
io_img.c
io_itv.c
io_mtm.c
io_showxpl.c			  
io_win.c
io_xpl.c
)
ADD_LIBRARY(triosio ${TRIOS_IO_SRCS})

INSTALL(TARGETS triosio
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

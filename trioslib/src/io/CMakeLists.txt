PROJECT(TRIOS_IO)
SET(TRIOS_IO_SRCS
io/io_header.c
io/io_imgset.c
io/io_img.c
io/io_itv.c
io/io_mtm.c
io/io_win.c
io/io_xpl.c
io/io_xpl_swt.c
#io/io_dt.c
)
ADD_LIBRARY(triosio ${TRIOS_IO_SRCS})
target_link_libraries(triosio triosbasic)

INSTALL(TARGETS triosio
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

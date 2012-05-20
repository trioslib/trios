PROJECT(PACBASIC)
SET(PACBASIC_SRCS 
pac_cmp.c 
pac_common.c 
pac_error.c 
pac_imgset.c 
pac_img.c
pac_math.c 
pac_show.c 
pac_win.c 
pac_xpl.c 
pac_xplBB.c)
ADD_LIBRARY(pacbasic ${PACBASIC_SRCS})

INSTALL(TARGETS pacbasic
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

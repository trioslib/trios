PROJECT(PACLEARN)
SET(PACLEARN_SRCS
ip_isi.c
#ip_isigray.c			   
#ip_mincover.c
#ip_newdec.c			   
#ip_shannon.c
#ip_switch.c			   
ipac_mtm.c
#ipac_mtmBG.c			   
#ipac_mtmGG.c
#dp_newdec.c
ip_group.c
dp_isi.c
dp_group.c
#dp_partition.c
#dp_itvconcat.c  
#dp_shannon.c
#ip_train2.c
# dp_train0.c
# dp_train1.c
#dp_switch.c
dpac_mtm.c       
#dp_isigray.c  
#dp_mincover.c
#dp_rank.c
)
ADD_LIBRARY(paclearn ${PACLEARN_SRCS})

INSTALL(TARGETS paclearn
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

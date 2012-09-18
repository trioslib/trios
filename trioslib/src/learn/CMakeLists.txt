PROJECT(TRIOS_LEARN)
SET(TRIOS_LEARN_SRCS
learn/ip_isi.c
#ip_isigray.c			   
learn/ip_mincover.c
#ip_newdec.c			   
#ip_shannon.c
#ip_switch.c			   
#ipac_mtmBG.c			   
#ipac_mtmGG.c
#dp_newdec.c
#ip_group.c
learn/dp_isi.c
#dp_group.c
#dp_partition.c
#dp_itvconcat.c  
#dp_shannon.c
#ip_train2.c
# dp_train0.c
# dp_train1.c
#dp_switch.c
learn/decision.c
learn/decision_bb.c
learn/multi_level.c
learn/mae.c
#dp_isigray.c  
#dp_mincover.c
#dp_rank.c
)
ADD_LIBRARY(trioslearn ${TRIOS_LEARN_SRCS})
target_link_libraries(trioslearn triosbasic triosio trios_collec triosappl m)

INSTALL(TARGETS trioslearn
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

PROJECT(TRIOS_LEARN)
SET(TRIOS_LEARN_SRCS
learn/ip_isi.c
learn/ip_mincover.c
learn/dp_isi.c
learn/decision.c
learn/decision_bb.c
learn/decision_gg.c
learn/multi_level.c

learn/accuracy.c
learn/operator.c
learn/train_gg.c
learn/partition.c

)
ADD_LIBRARY(trioslearn ${TRIOS_LEARN_SRCS})
target_link_libraries(trioslearn triosbasic triosio trios_collec triosappl trioscv m)

PROJECT(TRIOS_LEARN)
SET(TRIOS_LEARN_SRCS
learn/ip_isi.c
learn/ip_mincover.c
learn/dp_isi.c
learn/decision.c
learn/decision_bb.c
learn/decision_gg.c
learn/multi_level.c
learn/train_gg.c

learn/learn_dt/mktree.c
learn/learn_dt/tree_util.c
learn/learn_dt/impurity_measures.c
learn/learn_dt/compute_impurity.c
learn/learn_dt/perturb.c
learn/learn_dt/io_dt.c
learn/learn_dt/appl_gg.c

)
ADD_LIBRARY(trioslearn ${TRIOS_LEARN_SRCS})
target_link_libraries(trioslearn triosbasic triosio trios_collec triosappl trioscv m)


INSTALL(TARGETS trioslearn
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

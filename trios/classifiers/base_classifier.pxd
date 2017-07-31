from trios.serializable cimport Serializable

cdef class Classifier(Serializable):
    cdef public bint minimize, ordered, partial

    cpdef train(self, dataset, kw)
    cpdef partial_train(self, X, y, kw)
    cpdef apply(self, fvector)
    cpdef apply_batch(self, fmatrix)

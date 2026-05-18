#ifndef NUMPY_CORE_SRC_MULTIARRAY_CONVERT_H_
#define NUMPY_CORE_SRC_MULTIARRAY_CONVERT_H_

NPY_NO_EXPORT int
PyArray_AssignZero(PyArrayObject *dst,
                   PyArrayObject *wheremask);

NPY_NO_EXPORT PyObject *
PyArray_ViewDontFreeze(PyArrayObject *self);

#endif  /* NUMPY_CORE_SRC_MULTIARRAY_CONVERT_H_ */

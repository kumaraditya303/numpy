#include <Python.h>
#include <numpy/arrayobject.h>
#include <numpy/ufuncobject.h>

#if Py_LIMITED_API != PY_VERSION_HEX & 0xffff0000
    # error "Py_LIMITED_API not defined to Python major+minor version"
#endif

/*
 * Read a datetime descriptor's flags and c_metadata through the public
 * accessors and return (flags, unit, num).  Regression test for the 32-bit
 * free-threaded field layout, where c_metadata previously read back as NULL.
 */
static PyObject *limited_api_latest_datetime_metadata(PyObject *mod,
                                                      PyObject *arg)
{
    PyArrayObject *arr = (PyArrayObject *)arg;
    PyArray_Descr *descr = PyArray_DESCR(arr);
    npy_uint64 flags = PyDataType_FLAGS(descr);
    PyArray_DatetimeDTypeMetaData *dt_meta =
            (PyArray_DatetimeDTypeMetaData *)PyDataType_C_METADATA(descr);
    if (dt_meta == NULL) {
        PyErr_SetString(
            PyExc_RuntimeError,
            "PyDataType_C_METADATA() returned NULL for a datetime descriptor");
        return NULL;
    }
    return Py_BuildValue("Kii", (unsigned long long)flags,
                         (int)dt_meta->meta.base, dt_meta->meta.num);
}

static PyMethodDef limited_api_latest_methods[] = {
    {"datetime_metadata", (PyCFunction)limited_api_latest_datetime_metadata,
     METH_O,
     "Return (flags, unit, num) read from a datetime array's descriptor via "
     "the public descr accessors."},
    {NULL, NULL, 0, NULL}  /* Sentinel */
};

static PyModuleDef moduledef = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "limited_api_latest",
    .m_methods = limited_api_latest_methods
};

PyMODINIT_FUNC PyInit_limited_api_latest(void)
{
    import_array();
    import_umath();
    return PyModule_Create(&moduledef);
}

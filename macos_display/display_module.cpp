#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <pybind11/pybind11.h>
#include <string>
#include "display.h"

std::string display_name_wrapper(int x) {
    return getDisplayName((uint32_t) x);
}

PYBIND11_MODULE(display, m) {
    m.def("display_name", &display_name_wrapper);
}
#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <pybind11/pybind11.h>

#include <string>
#include <cstdlib>

#include "display.h"

std::string display_name_wrapper(int x) {
    char* c = getDisplayName((uint32_t) x);
    std::string s{c};
    free(c);
    return s;
}

std::string display_mode_wrapper(int x) {
    char* c = getDisplayMode((uint32_t) x);
    std::string s{c};
    free(c);
    return s;
}

PYBIND11_MODULE(display, m) {
    m.def("display_name", &display_name_wrapper);
    m.def("display_mode", &display_mode_wrapper);
}
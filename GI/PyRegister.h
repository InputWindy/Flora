#pragma once
#include <CoreMinimal.h>
#include <pybind11/pybind11.h>

using namespace flora;
namespace py = pybind11;

typedef bool(*pybind_register_func)(pybind11::module& m);
struct PyRegister :public flora::XSingleton<PyRegister>
{
    static inline bool Register(pybind_register_func func) { if (func)register_funcs.push_back(func); return true; }

    static inline void Exec(pybind11::module& m)
    {
        for (pybind_register_func f : register_funcs)
        {
            f(m);
        }
    }

    static inline std::vector<pybind_register_func> register_funcs;
};

#define PY_REGISTER(PyClassName, ClassInfo,...) \
static bool PyClassName##RegisterFunc(pybind11::module& m) \
{ \
    py::class_<__VA_ARGS__>(m, #PyClassName) \
        ClassInfo; \
    return true; \
} \
static inline bool b##PyClassName##Registerd = PyRegister::Get().Register(PyClassName##RegisterFunc);

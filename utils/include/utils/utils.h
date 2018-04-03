#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

// GCC specfic method to get a rough representation of the absolute name of the type T
// This is just a helper function for debugging.
template <typename T>
void PrintTypeName(const T&)
{
#ifdef __GNUG__
    int status;
    std::string realname = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    std::cout << realname << std::endl;
#else
#error "Function only implemented for GCC"
#endif
}

#endif /* UTILS_H */

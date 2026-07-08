#pragma once

#include <string>

#define GET_NAME(name) GetNameFromVariable(#name)

inline std::string GetNameFromVariable(const char* name)
{
    return name;
}
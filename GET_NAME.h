#pragma once

#include <string>

namespace chess
{

#define GET_NAME(name) GetNameFromVariable(#name)

inline std::string GetNameFromVariable(const char* name)
{
    return name;
}

} // namespace chess
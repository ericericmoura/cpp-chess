#pragma once

#include <string>

#define GET_NAME(name) GetNameFromVariable(#name)

std::string GetNameFromVariable(const char* name)
{
    return name;
}
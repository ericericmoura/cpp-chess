#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <regex>

namespace chess
{
namespace file_io
{

inline std::pair<std::string, std::string> GetFileTagByRegex(const std::string& line)
{
    std::regex pattern(R"(\[(\w+)\](.*?)\[\1\])");

    std::smatch match;
    if (std::regex_search(line, match, pattern))
    {
        std::string tag = match[1].str();
        std::string content = match[2].str();
        return { match[1], match[2] };
    }
#ifdef _DEBUG
    std::cout << "\nNo tag found for string: " << line << "\n";
#endif // _DEBUG
    return {};
}

} // namespace file_io
} // namespace chess
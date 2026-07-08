#pragma once

#include <sstream>
#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>

template <typename T>
inline sf::Vector2<T> StringToVector2(const std::string& value)
{
    std::stringstream ss(value);
    std::vector<std::string> result;

    while (ss.good())
    {
        std::string substr{};
        getline(ss, substr, ',');
        result.push_back(substr);
    }

    sf::Vector2<T> vector{};
    if (result.size() < 2)
    {
        return{};
    }
    vector.x = static_cast<T>(std::stof(result.at(0)));
    vector.y = static_cast<T>(std::stof(result.at(1)));
    return vector;
}
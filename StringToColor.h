#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cctype>

sf::Color StringToColor(const std::string& value) noexcept
{
	std::stringstream ss(value);
    std::vector<std::string> result;

    while (ss.good())
    {
        std::string substr{};
        getline(ss, substr, ',');
        result.push_back(substr);
    }

    sf::Color color_{};
    if (result.size() < 3)
    {
        return{};
    }
    color_.r = std::stoi(result.at(0));
    color_.g = std::stoi(result.at(1));
    color_.b = std::stoi(result.at(2));
    if (result.size() == 4)
    {
        color_.a = std::stoi(result.at(3));
    }
    return color_;
}
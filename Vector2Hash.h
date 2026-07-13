#pragma once

#include <type_traits>

#include <SFML/System/Vector2.hpp>

namespace chess
{

struct Vec2uHash {
    std::size_t operator()(const sf::Vector2u& v) const {
        return std::hash<unsigned>()(v.x) ^ (std::hash<unsigned>()(v.y) << 1);
    }
};

} // namespace chess
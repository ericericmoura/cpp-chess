#pragma once

#include <SFML/System/Vector2.hpp>

namespace chess {

class Board
{
public:
	inline static constexpr sf::Vector2i kBoardSize = { 8 , 8  };
	inline static constexpr sf::Vector2i kCellSize  = { 64, 64 };

private:
};

}
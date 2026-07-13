#pragma once

#include <SFML/System/Vector2.hpp>

namespace chess
{

struct GeneratedBoardInfo
{
	sf::Vector2u white_king_coords_{};
	sf::Vector2u black_king_coords_{};
};

} // namespace chess
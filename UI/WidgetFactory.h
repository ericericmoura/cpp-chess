#pragma once

#include <functional>

#include <SFML/System/Vector2.hpp>

#include "ColorRect.h"
#include "../Team.h"

namespace chess
{
namespace ui
{

class WidgetFactory
{
public:
	ColorRect CreatePromotionWidget(
		Team team, 
		sf::Vector2f position, 
		std::function<void()> knight_chosen_cb,
		std::function<void()> bishop_chosen_cb,
		std::function<void()> rook_chosen_cb,
		std::function<void()> queen_chosen_cb);
};

} // namespace ui
} // namespace chess
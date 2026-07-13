#pragma once

#include <SFML/System/Vector2.hpp>

#include "MovementComponent.h"

namespace chess
{

class Piece;
class Board;

class LShapedMovementComponent final: public MovementComponent
{
public:
	LShapedMovementComponent(Piece& piece) noexcept;

private:
	bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept override;
	bool IsPositionBlocked  (const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, Board& board) const noexcept override;
};

} // namespace chess
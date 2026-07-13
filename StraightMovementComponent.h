#pragma once

#include <SFML/System/Vector2.hpp>

#include "MovementComponent.h"

namespace chess
{

class Piece;
class Board;

class StraightMovementComponent : public MovementComponent
{
public:
	StraightMovementComponent(Piece& piece, unsigned int movement_range = MovementComponent::kUnlimitedMovementRange);

protected:
	unsigned int movement_range_ = 0;	

	virtual bool IsPositionReachable(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, bool occupied_by_enemy) const noexcept override;
	virtual bool IsPositionBlocked  (const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, Board& board) const noexcept override;
};

} // namespace chess
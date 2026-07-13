#pragma once

#include <SFML/System/Vector2.hpp>

#include "MovementComponent.h"

namespace chess
{

class Piece;
class Board;

class DiagonalMovementComponent : public MovementComponent
{
public:
	DiagonalMovementComponent(Piece& piece, unsigned int movement_range = MovementComponent::kUnlimitedMovementRange) noexcept;

private:
	unsigned int movement_range_ = 0;

	bool IsPositionReachable(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, bool occupied_by_enemy) const noexcept override;
	bool IsPositionBlocked(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, Board& board) const noexcept override;
};

} // namespace chess
#pragma once

#include "MovementComponent.h"

namespace chess
{

class DiagonalMovementComponent : public MovementComponent
{
public:
	DiagonalMovementComponent(Piece& piece, unsigned int movement_range = MovementComponent::kUnlimitedMovementRange) noexcept;

private:
	unsigned int movement_range_ = 0;

	bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept override;
	bool IsPositionBlocked(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, Board& board) const noexcept override;
};

}
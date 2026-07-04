#pragma once

#include <SFML/System/Vector2.hpp>

#include "MovementComponent.h"

namespace chess
{

class Piece;

class StraightMovementComponent : public MovementComponent
{
public:
	StraightMovementComponent(Piece& piece, unsigned int movement_range = MovementComponent::kUnlimitedMovementRange);

	virtual bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept override;
	virtual bool IsPositionBlocked  (const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, PiecesMap& pieces     ) const noexcept override;

protected:
	unsigned int movement_range_ = 0;	
};

}
#pragma once

#include <SFML/System/Vector2.hpp>

#include "MovementComponent.h"
#include "Piece.h"
#include "Team.h"

namespace chess
{

class StraightMovementComponent : public MovementComponent
{
public:
	StraightMovementComponent(Team team, unsigned int movement_range = MovementComponent::kUnlimitedMovementRange)
		: MovementComponent(team)
		, movement_range_(movement_range)
	{}

	virtual bool TryMove(
		Piece& piece, 
		const sf::Vector2u& current_pos, 
		const sf::Vector2u& target_pos, 
		PiecesMap& pieces) noexcept override;

	virtual bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept override;
	virtual bool IsPositionBlocked(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, PiecesMap& pieces) const noexcept override;

	virtual void Moved() noexcept {};

protected:
	unsigned int movement_range_ = 0;	
};

}
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
	StraightMovementComponent(Team team)
		: MovementComponent(team)
	{}

	bool TryMove(
		Piece& piece, 
		const sf::Vector2u& current_pos, 
		const sf::Vector2u& target_pos, 
		PiecesMap& pieces) const noexcept override;

	bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos) const noexcept override;
	bool IsPositionBlocked(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, PiecesMap& pieces) const noexcept override;
};

}
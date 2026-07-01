#pragma once

#include <unordered_map>

#include <SFML/System/Vector2.hpp>

#include "MovementComponent.h"
#include "Piece.h"

namespace chess
{

class StraightMovementComponent : public MovementComponent
{
public:
	bool TryMove(
		const Piece& piece, 
		const sf::Vector2i& current_pos, 
		const sf::Vector2i& target_pos, 
		std::unordered_map<sf::Vector2i, Piece>& pieces) const noexcept override;

	// Inherited via MovementComponent
	bool IsPositionReachable(const sf::Vector2i& current_pos, const sf::Vector2i& target_pos) const noexcept override;
	bool IsPositionBlocked(const sf::Vector2i& current_pos, const sf::Vector2i& target_pos, std::unordered_map<sf::Vector2i, Piece>& pieces) const noexcept override;
};

}
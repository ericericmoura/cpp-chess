#pragma once

#include <SFML/System/Vector2.hpp>

#include "Team.h"
#include "PieceType.h"

namespace chess {

class Piece;
class Board;

class MovementComponent
{
public:
	static inline constexpr unsigned int kUnlimitedMovementRange = 0;

	MovementComponent(Piece& piece);

	virtual bool CanMoveTo(		 
		const Board& board,
		const sf::Vector2u& current_coords, 
		const sf::Vector2u& target_coords) const noexcept;

	virtual void Moved(
		const sf::Vector2u& previous_coords,
		const sf::Vector2u& current_coords,
		Board& board) noexcept {};

protected:
	Team      team_{};
	PieceType type_{};

	virtual bool IsPositionReachable(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, bool occupied_by_enemy) const noexcept = 0;
	virtual bool IsPositionBlocked  (const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, const Board& board) const noexcept = 0;
};

} // namespace chess
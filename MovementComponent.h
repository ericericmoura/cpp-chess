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

	virtual bool TryMove(		 
		Board& board,
		const sf::Vector2u& current_pos, 
		const sf::Vector2u& target_pos) noexcept;

protected:
	Team      team_{};
	PieceType type_{};

	virtual bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept = 0;
	virtual bool IsPositionBlocked  (const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, Board& board) const noexcept = 0;

	virtual void Moved(
		const sf::Vector2u& previous_pos,
		const sf::Vector2u& current_pos,
		Board& board) noexcept {};
};

}
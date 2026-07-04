#pragma once

#include <unordered_map>

#include <SFML/System/Vector2.hpp>

#include "Team.h"
#include "Vector2Hash.h"
#include "PieceType.h"

namespace chess {

class Piece;

using PiecesMap = std::unordered_map<sf::Vector2u, Piece, Vec2uHash>;

class MovementComponent
{
public:
	static inline constexpr unsigned int kUnlimitedMovementRange = 0;

	MovementComponent(Piece& piece);

	bool TryMove(		 
		const sf::Vector2u& current_pos, 
		const sf::Vector2u& target_pos, 
		PiecesMap& pieces) noexcept;

protected:
	Team      team_{};
	PieceType type_{};

	virtual bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept = 0;
	virtual bool IsPositionBlocked  (const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, PiecesMap& pieces     ) const noexcept = 0;

	virtual void Moved(
		const sf::Vector2u& previous_pos,
		const sf::Vector2u& current_pos,
		PiecesMap& pieces) noexcept {};
};

}
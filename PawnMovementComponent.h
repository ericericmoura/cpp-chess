#pragma once

#include <SFML/System/Vector2.hpp>

#include "StraightMovementComponent.h"

namespace chess
{

class Piece;
class Board;

class PawnMovementComponent : public StraightMovementComponent
{
public:
	PawnMovementComponent(Piece& piece);

	virtual bool TryMove(
		Board& board,
		const sf::Vector2u& current_pos,
		const sf::Vector2u& target_pos) noexcept override;
	
	bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept override;
	void AllowEnPassant(sf::Vector2u at_pos) noexcept;

private:
	bool first_movement_ = true;

	mutable bool done_en_passant_   = false;
	mutable bool can_do_en_passant_ = false;
	sf::Vector2u en_passant_position_      {};
	sf::Vector2u en_passant_piece_position_{};

	void Moved(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, Board& board) noexcept override;
	void DecreasePawnMovement() noexcept;
	void CheckForEnPassant   (const sf::Vector2u& previous_pos, const sf::Vector2u& current_pos, Board& board) noexcept;
};

}

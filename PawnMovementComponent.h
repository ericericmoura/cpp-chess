#pragma once

#include <optional>

#include <SFML/System/Vector2.hpp>

#include "StraightMovementComponent.h"

namespace chess
{

class Piece;
class Board;

class PawnMovementComponent final : public StraightMovementComponent
{
public:
	PawnMovementComponent(Piece& piece);

	void Moved(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, Board& board) noexcept override;

private:
	bool first_movement_ = true;

	mutable std::optional<sf::Vector2u> en_passant_piece_coords_ {};
	mutable std::optional<sf::Vector2u> en_passant_target_coords_{};

	void DecreasePawnMovement() noexcept;
	
	void CheckForEnPassant(const sf::Vector2u& previous_coords, const sf::Vector2u& current_coords, Board& board) noexcept;
	void AllowEnPassant(const Board& board, sf::Vector2u at_pos) noexcept;

	bool IsPositionReachable(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, bool occupied_by_enemy) const noexcept override;
};

} // namespace chess

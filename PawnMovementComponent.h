#pragma once

#include <SFML/System/Vector2.hpp>

#include "StraightMovementComponent.h"

namespace chess
{

class Piece;

class PawnMovementComponent : public StraightMovementComponent
{
public:
	PawnMovementComponent(Piece& piece);
	
	bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept override;
	void AllowEnPassant(sf::Vector2u at_pos) noexcept;

private:
	bool first_movement_ = true;

	bool         can_do_en_passant_ = false;
	sf::Vector2u en_passant_position_{};

	void Moved(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, PiecesMap& pieces) noexcept override;
};

}

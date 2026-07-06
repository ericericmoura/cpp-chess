#include "DiagonalMovementComponent.h"

chess::DiagonalMovementComponent::DiagonalMovementComponent(Piece& piece, unsigned int movement_range) noexcept
    : MovementComponent(piece)
    , movement_range_(movement_range)
{}

bool chess::DiagonalMovementComponent::IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept
{
    return false;
}

bool chess::DiagonalMovementComponent::IsPositionBlocked(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, Board& board) const noexcept
{
    return false;
}

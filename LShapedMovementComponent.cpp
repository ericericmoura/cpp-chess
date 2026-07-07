#include "LShapedMovementComponent.h"

#include <cmath>

#include <SFML/System/Vector2.hpp>

#include "MovementComponent.h"

chess::LShapedMovementComponent::LShapedMovementComponent(Piece& piece) noexcept
    : MovementComponent(piece)
{}

bool chess::LShapedMovementComponent::IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept
{
    auto distance_x = abs(static_cast<int>(target_pos.x) - static_cast<int>(current_pos.x));
    auto distance_y = abs(static_cast<int>(target_pos.y) - static_cast<int>(current_pos.y));

    return distance_x == 2 && distance_y == 1 ||
           distance_x == 1 && distance_y == 2;
}

bool chess::LShapedMovementComponent::IsPositionBlocked(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, Board& board) const noexcept
{
    return false;
}

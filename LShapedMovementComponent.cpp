#include "LShapedMovementComponent.h"

#include <cmath>

#include <SFML/System/Vector2.hpp>

#include "MovementComponent.h"

chess::LShapedMovementComponent::LShapedMovementComponent(Piece& piece) noexcept
    : MovementComponent(piece)
{}

bool chess::LShapedMovementComponent::IsPositionReachable(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, bool occupied_by_enemy) const noexcept
{
    auto distance_x = abs(static_cast<int>(target_coords.x) - static_cast<int>(current_coords.x));
    auto distance_y = abs(static_cast<int>(target_coords.y) - static_cast<int>(current_coords.y));

    return distance_x == 2 && distance_y == 1 ||
           distance_x == 1 && distance_y == 2;
}

bool chess::LShapedMovementComponent::IsPositionBlocked(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, Board& board) const noexcept
{
    return false;
}

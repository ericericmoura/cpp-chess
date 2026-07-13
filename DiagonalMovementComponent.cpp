#include "DiagonalMovementComponent.h"

#include <cmath>

#include <SFML/System/Vector2.hpp>

#include "MovementComponent.h"
#include "Piece.h"
#include "Board.h"

chess::DiagonalMovementComponent::DiagonalMovementComponent(Piece& piece, unsigned int movement_range) noexcept
    : MovementComponent(piece)
    , movement_range_(movement_range)
{}

bool chess::DiagonalMovementComponent::IsPositionReachable(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, bool occupied_by_enemy) const noexcept
{
    auto distance_x = abs(static_cast<int>(target_coords.x) - static_cast<int>(current_coords.x));
    auto distance_y = abs(static_cast<int>(target_coords.y) - static_cast<int>(current_coords.y));

    auto is_in_range = (movement_range_ == MovementComponent::kUnlimitedMovementRange || distance_x <= movement_range_);
    auto is_diagonal = distance_x == distance_y;

    return is_diagonal && is_in_range;
}

bool chess::DiagonalMovementComponent::IsPositionBlocked(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, Board& board) const noexcept
{
    auto direction = sf::Vector2i(target_coords) - sf::Vector2i(current_coords);
    direction.x /= abs(direction.x);
    direction.y /= abs(direction.y);

    auto start_pos = sf::Vector2i(current_coords) + direction;
    auto end_pos   = target_coords;

    for (auto x = start_pos.x, y = start_pos.y; x != end_pos.x; x += direction.x, y += direction.y)
    {
        auto pos = sf::Vector2u({ static_cast<unsigned int>(x), static_cast<unsigned int>(y) });
        if (board.IsCoordinatesOccupied(pos))
        {
            return true;
        }
    }
    return false;
}

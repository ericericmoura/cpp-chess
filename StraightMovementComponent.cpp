#include "StraightMovementComponent.h"

#include <algorithm>
#include <cmath>

#include <SFML/System/Vector2.hpp>

#include "MovementComponent.h"
#include "Piece.h"
#include "Board.h"

chess::StraightMovementComponent::StraightMovementComponent(Piece& piece, unsigned int movement_range)
    : MovementComponent(piece)
    , movement_range_(movement_range)
{}

bool chess::StraightMovementComponent::IsPositionReachable(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, bool occupied_by_enemy) const noexcept
{
    if (movement_range_ != MovementComponent::kUnlimitedMovementRange)
    {
        auto distance_x = abs(static_cast<int>(current_coords.x - target_coords.x));
        auto distance_y = abs(static_cast<int>(current_coords.y - target_coords.y));

        if (distance_x > movement_range_ || distance_y > movement_range_)
        {
            return false;
        }
    }
    return target_coords.x == current_coords.x || target_coords.y == current_coords.y;
}

bool chess::StraightMovementComponent::IsPositionBlocked(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, const Board& board) const noexcept
{
    auto is_vertical = target_coords.x == current_coords.x;

    const unsigned current_coord = is_vertical ? current_coords.y : current_coords.x;
    const unsigned target_coord  = is_vertical ? target_coords.y : target_coords.x;

    const unsigned lower = std::min(current_coord, target_coord) + 1;
    const unsigned upper = std::max(current_coord, target_coord);

    for (unsigned i = lower; i < upper; ++i)
    {
        auto pos = is_vertical ? sf::Vector2u(current_coords.x, i) 
                               : sf::Vector2u(i, current_coords.y);
        if (board.IsCoordinatesOccupied(pos))
        {
            return true;
        }
    }
    return false;
}

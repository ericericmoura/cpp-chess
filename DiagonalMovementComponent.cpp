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

bool chess::DiagonalMovementComponent::IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept
{
    auto distance_x = abs(static_cast<int>(target_pos.x) - static_cast<int>(current_pos.x));
    auto distance_y = abs(static_cast<int>(target_pos.y) - static_cast<int>(current_pos.y));

    auto is_in_range = (movement_range_ == MovementComponent::kUnlimitedMovementRange || distance_x <= movement_range_);
    auto is_diagonal = distance_x == distance_y;

    return is_diagonal && is_in_range;
}

bool chess::DiagonalMovementComponent::IsPositionBlocked(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, Board& board) const noexcept
{
    auto direction = sf::Vector2i(target_pos) - sf::Vector2i(current_pos);
    direction.x /= abs(direction.x);
    direction.y /= abs(direction.y);

    auto start_pos = sf::Vector2i(current_pos) + direction;
    auto end_pos   = target_pos;

    for (auto x = start_pos.x, y = start_pos.y; x != end_pos.x; x += direction.x, y += direction.y)
    {
        auto pos = sf::Vector2u({ static_cast<unsigned int>(x), static_cast<unsigned int>(y) });
        if (board.IsCoordinateOccupied(pos))
        {
            return true;
        }
    }
    return false;
}

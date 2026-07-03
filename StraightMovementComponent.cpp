#include "StraightMovementComponent.h"

#include <unordered_map>
#include <utility>

#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "MovementComponent.h"

bool chess::StraightMovementComponent::TryMove(
    Piece& piece, 
    const sf::Vector2u& current_pos, 
    const sf::Vector2u& target_pos, 
    PiecesMap& pieces) const noexcept
{
    auto it = pieces.find(target_pos);
    bool occupied_by_ally = it != pieces.end() && it->second.GetTeam() == team_;
    bool is_position_valid =
           !occupied_by_ally
        && IsPositionReachable(current_pos, target_pos)
        && !IsPositionBlocked(current_pos, target_pos, pieces);
    
    return is_position_valid;
}

bool chess::StraightMovementComponent::IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos) const noexcept
{
    return target_pos.x == current_pos.x || target_pos.y == current_pos.y;
}

bool chess::StraightMovementComponent::IsPositionBlocked(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, PiecesMap& pieces) const noexcept
{
    bool vertical = target_pos.x == current_pos.x;

    auto start_i = current_pos.x + 1,
        target_i = target_pos.x;

    if (vertical)
    {
        start_i  = current_pos.y + 1;
        target_i = target_pos.y;
    }

    if (start_i > target_i)
    {
        auto temp = start_i;
        start_i  = target_i + 1;
        target_i = temp  - 1;
    }

    for (int i = start_i; i < target_i; ++i)
    {
        auto pos = vertical ? sf::Vector2u(current_pos.x, i) : sf::Vector2u(i, current_pos.y);
        auto it_checking = pieces.find(pos);
        if (it_checking != pieces.end())
        {
            return true;
        }
    }
    return false;
}

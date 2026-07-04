#include "MovementComponent.h"

#include <SFML/System/Vector2.hpp>

#include "Piece.h"

chess::MovementComponent::MovementComponent(Piece& piece)
    : team_(piece.GetTeam())
    , type_(piece.GetPieceType())
{}

bool chess::MovementComponent::TryMove(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, PiecesMap& pieces) noexcept
{
    auto it = pieces.find(target_pos);
    bool occupied_by_ally  = it != pieces.end() && it->second.GetTeam() == team_;
    bool occupied_by_enemy = it != pieces.end() && it->second.GetTeam() != team_;

    bool is_position_valid =
        !occupied_by_ally
        && IsPositionReachable(current_pos, target_pos, occupied_by_enemy)
        && !IsPositionBlocked(current_pos, target_pos, pieces);

    if (is_position_valid)
    {
        Moved(current_pos, target_pos, pieces);
    }

    return is_position_valid;
}

bool chess::MovementComponent::TryMove(const sf::Vector2u& previous_pos, const sf::Vector2u& current_pos, PiecesMap& pieces) noexcept
{
    return false;
}

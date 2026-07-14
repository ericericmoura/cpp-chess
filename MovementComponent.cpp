#include "MovementComponent.h"

#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Board.h"

chess::MovementComponent::MovementComponent(Piece& piece)
    : team_(piece.GetTeam())
    , type_(piece.GetPieceType())
{}

bool chess::MovementComponent::CanMoveTo(const Board & board, const sf::Vector2u & current_pos, const sf::Vector2u & target_pos) const noexcept
{    
    auto target_piece = board.GetPieceAtCoordinates(target_pos);

    bool occupied_by_ally  = false;
    bool occupied_by_enemy = false;
    if (target_piece != nullptr)
    {
        occupied_by_ally  = target_piece->GetTeam() == team_;
        occupied_by_enemy = target_piece->GetTeam() != team_;
    }

    bool is_position_valid =
        !occupied_by_ally
        && IsPositionReachable(current_pos, target_pos, occupied_by_enemy)
        && !IsPositionBlocked(current_pos, target_pos, board);

    return is_position_valid;
}
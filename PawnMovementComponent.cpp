#include "PawnMovementComponent.h"

#include <cmath>

#include <SFML/System/Vector2.hpp>

#include "StraightMovementComponent.h"
#include "MovementComponent.h"
#include "Piece.h"
#include "Team.h"
#include "PieceType.h"
#include "Board.h"

chess::PawnMovementComponent::PawnMovementComponent(Piece& piece)
    : StraightMovementComponent(piece, 2)
{};

bool chess::PawnMovementComponent::IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept
{   
    auto distance_y = static_cast<int>(target_pos.y - current_pos.y);
    bool going_forwards = team_ == chess::Team::White ? distance_y < 0 : distance_y > 0;
    if (!going_forwards)
    {
        return false;
    }
    if (!occupied_by_enemy)
    {
        return StraightMovementComponent::IsPositionReachable(current_pos, target_pos, occupied_by_enemy);
    }
    auto distance_x = static_cast<int>(target_pos.x - current_pos.x);

    return abs(distance_x) == abs(distance_y);
}

void chess::PawnMovementComponent::AllowEnPassant(sf::Vector2u at_pos) noexcept
{
    if (!chess::Board::IsCoordinatesValid(at_pos))
    {
        return;
    }
    can_do_en_passant_   = true;
    en_passant_position_ = at_pos;
}

void chess::PawnMovementComponent::Moved(const sf::Vector2u& previous_pos, const sf::Vector2u& current_pos, PiecesMap& pieces) noexcept
{
    auto left_pos  = previous_pos;
    auto right_pos = previous_pos;

    left_pos .x--;
    right_pos.x--;

    auto it_left  = pieces.find(left_pos );
    if (it_left != pieces.end() && it_left->second.GetPieceType() == chess::PieceType::Pawn)
    {
        auto pawn_movement_component = it_left->second.GetComponentByType<PawnMovementComponent>();
        if (pawn_movement_component)
        {
            auto en_passant_position = current_pos;
            en_passant_position.y -= team_ == Team::Black ? 1 : -1;
            pawn_movement_component->AllowEnPassant(en_passant_position);
        }
    }

    auto it_right = pieces.find(right_pos);

    if (!first_movement_)
    {
        return;
    }
    first_movement_ = false;
    movement_range_--;
}

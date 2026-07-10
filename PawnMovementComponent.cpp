#include "PawnMovementComponent.h"

#include <cmath>

#include <SFML/System/Vector2.hpp>

#include "StraightMovementComponent.h"
#include "MovementComponent.h"
#include "Piece.h"
#include "Team.h"
#include "PieceType.h"
#include "Board.h"
#include <utility>

chess::PawnMovementComponent::PawnMovementComponent(Piece& piece)
    : StraightMovementComponent(piece, 2)
{}

bool chess::PawnMovementComponent::TryMove(Board& board, const sf::Vector2u & current_pos, const sf::Vector2u & target_pos) noexcept
{
    bool result = MovementComponent::TryMove(board, current_pos, target_pos);

    if (done_en_passant_)
    {
        board.CaptureAtCoordinates(en_passant_piece_position_);
        done_en_passant_ = false;
    }

    return result;
}

bool chess::PawnMovementComponent::IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept
{   
    auto distance_y = static_cast<int>(target_pos.y - current_pos.y);
    bool going_forwards = team_ == chess::Team::White ? distance_y < 0 : distance_y > 0;
    if (!going_forwards)
    {
        return false;
    }
    if (can_do_en_passant_ && target_pos == en_passant_position_)
    {
        done_en_passant_   = true;
        can_do_en_passant_ = false;
        return true;
    }
    if (!occupied_by_enemy)
    {
        return StraightMovementComponent::IsPositionReachable(current_pos, target_pos, occupied_by_enemy);
    }
    auto distance_x = static_cast<int>(target_pos.x - current_pos.x);

    return abs(distance_x) == abs(distance_y);
}

void chess::PawnMovementComponent::AllowEnPassant(const Board& board, sf::Vector2u at_pos) noexcept
{
    if (!board.IsCoordinatesWithinBounds(at_pos))
    {
        return;
    }
    can_do_en_passant_   = true;
    en_passant_position_ = std::move(at_pos);
}

void chess::PawnMovementComponent::Moved(const sf::Vector2u& previous_pos, const sf::Vector2u& current_pos, Board& board) noexcept
{
    auto distance_y = abs(static_cast<int>(current_pos.y - previous_pos.y));
    if (first_movement_ && distance_y == 2)
    {
        auto left_pos  = current_pos;
        auto right_pos = current_pos;

        left_pos.x--;
        right_pos.x++;

        CheckForEnPassant(left_pos , current_pos, board);
        CheckForEnPassant(right_pos, current_pos, board);
    }
    DecreasePawnMovement();
}

void chess::PawnMovementComponent::DecreasePawnMovement() noexcept
{
    if (!first_movement_)
    {
        return;
    }
    first_movement_ = false;
    movement_range_--;
}

void chess::PawnMovementComponent::CheckForEnPassant(const sf::Vector2u& target_pos, const sf::Vector2u& current_pos, Board& board) noexcept
{    
    auto nearby_piece = board.GetPieceAtCoordinates(target_pos);
    if (nearby_piece == nullptr || nearby_piece->GetPieceType() != chess::PieceType::Pawn)
    {
        return;
    }
    auto pawn_movement_component = nearby_piece->GetComponentByType<PawnMovementComponent>();
    if (pawn_movement_component == nullptr)
    {
        return;
    }
    auto en_passant_position = current_pos;
    en_passant_position.y -= team_ == Team::Black ? 1 : -1;
    pawn_movement_component->AllowEnPassant(board, en_passant_position);
    pawn_movement_component->en_passant_piece_position_ = current_pos;
}

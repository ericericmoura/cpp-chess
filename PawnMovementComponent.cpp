#include "PawnMovementComponent.h"

#include <cmath>
#include <utility>

#include <SFML/System/Vector2.hpp>

#include "StraightMovementComponent.h"
#include "MovementComponent.h"
#include "Piece.h"
#include "Team.h"
#include "PieceType.h"
#include "Board.h"

chess::PawnMovementComponent::PawnMovementComponent(Piece& piece)
    : StraightMovementComponent(piece, 2)
{}

bool chess::PawnMovementComponent::IsPositionReachable(const sf::Vector2u& current_coords, const sf::Vector2u& target_coords, bool occupied_by_enemy) const noexcept
{   
    auto distance_y = static_cast<int>(target_coords.y - current_coords.y);
    bool going_forwards = team_ == chess::Team::White ? distance_y < 0 : distance_y > 0;
    if (!going_forwards)
    {
        return false;
    }
    if (en_passant_piece_coords_.has_value() && target_coords == en_passant_target_coords_)
    {
        return true;
    }
    if (!occupied_by_enemy)
    {
        return StraightMovementComponent::IsPositionReachable(current_coords, target_coords, occupied_by_enemy);
    }
    auto distance_x = static_cast<int>(target_coords.x - current_coords.x);

    return abs(distance_x) == 1 && abs(distance_x) == abs(distance_y);
}

void chess::PawnMovementComponent::AllowEnPassant(const Board& board, sf::Vector2u at_pos) noexcept
{
    if (!board.IsCoordinatesWithinBounds(at_pos))
    {
        return;
    }
    en_passant_target_coords_ = std::move(at_pos);
}

void chess::PawnMovementComponent::Moved(const sf::Vector2u& previous_coords, const sf::Vector2u& current_coords, Board& board) noexcept
{
    if (en_passant_piece_coords_.has_value() && en_passant_target_coords_.has_value())
    {
        board.CaptureAtCoordinates(en_passant_piece_coords_.value());
        en_passant_target_coords_ = {};
        en_passant_piece_coords_  = {};
        return;
    }
    auto distance_y = abs(static_cast<int>(current_coords.y - previous_coords.y));
    if (first_movement_ && distance_y == 2)
    {
        auto left_pos  = current_coords;
        auto right_pos = current_coords;

        left_pos.x--;
        right_pos.x++;

        CheckForEnPassant(left_pos , current_coords, board);
        CheckForEnPassant(right_pos, current_coords, board);
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

void chess::PawnMovementComponent::CheckForEnPassant(const sf::Vector2u& target_coords, const sf::Vector2u& current_coords, Board& board) noexcept
{    
    auto nearby_piece = board.GetPieceAtCoordinates(target_coords);
    if (nearby_piece == nullptr || nearby_piece->GetPieceType() != chess::PieceType::Pawn)
    {
        return;
    }
    auto pawn_movement_component = nearby_piece->GetComponentByType<PawnMovementComponent>();
    if (pawn_movement_component == nullptr)
    {
        return;
    }
    auto en_passant_position = current_coords;
    en_passant_position.y -= team_ == Team::Black ? 1 : -1;
    pawn_movement_component->AllowEnPassant(board, en_passant_position);
    pawn_movement_component->en_passant_piece_coords_ = current_coords;
}

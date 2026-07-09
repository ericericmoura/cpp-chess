#include "PieceFactory.h"

#include <cmath>
#include <memory>
#include <utility>

#include <SFML/System/Vector2.hpp>

#include "StraightMovementComponent.h"
#include "MovementComponent.h"
#include "PawnMovementComponent.h"
#include "DiagonalMovementComponent.h"
#include "LShapedMovementComponent.h"
#include "Board.h"
#include "Piece.h"
#include "Team.h"
#include "PieceType.h"

void chess::PieceFactory::GeneratePieces(const Board& board, PiecesMap& map) const noexcept
{
	for (auto i = 0; i < 16; i++)
	{
		auto index = static_cast<float>(i);
		auto grid_y = static_cast<unsigned int>(floor(index / board.GetConfig().board_size_.x));
		auto grid_x = i % board.GetConfig().board_size_.x;

		auto black_position = sf::Vector2u({ grid_x, grid_y });
		auto white_position = black_position;
		white_position.y += 6;

		auto black_piece = GeneratePiece(board, black_position, chess::Team::Black);
		auto white_piece = GeneratePiece(board, white_position, chess::Team::White);

		map.try_emplace(black_position, std::move(black_piece));
		map.try_emplace(white_position, std::move(white_piece));
	}
}

chess::PieceType chess::PieceFactory::GetPieceType(const sf::Vector2u& position) const noexcept
{
	if (position.y == 1 || position.y == 6)
	{
		return PieceType::Pawn;
	}
	if (position.x == 0 || position.x == 7)
	{
		return PieceType::Rook;
	}
	if (position.x == 1 || position.x == 6)
	{
		return PieceType::Knight;
	}
	if (position.x == 2 || position.x == 5)
	{
		return PieceType::Bishop;
	}
	if (position.x == 2 || position.x == 5)
	{
		return PieceType::Bishop;
	}
	if (position.x == 3)
	{
		return PieceType::Queen;
	}
	return PieceType::King;
}

chess::Piece chess::PieceFactory::GeneratePiece(const Board& board, const sf::Vector2u& position, chess::Team team) const noexcept
{
	// Get type
	auto type = GetPieceType(position);
	
	// Get image extension
	auto& team_name = team == chess::Team::White ? pieces_info.white_team_name_ : pieces_info.black_team_name_;
	auto piece_path = pieces_info.file_path_ + team_name + pieces_info.piece_name_map_.at(type) + pieces_info.sprite_extension_;
	auto piece = chess::Piece(team, piece_path);

	piece.SetBoardPosition(board, position);
	
	AttachComponentsForPiece(piece, team, type);

	return piece;
}

void chess::PieceFactory::AttachComponentsForPiece(Piece& piece, chess::Team team, chess::PieceType type) const noexcept
{
	if (type == chess::PieceType::Pawn)
	{
		piece.AddMovementComponent(std::make_unique<PawnMovementComponent>(piece));
	}
	else if (type == chess::PieceType::Rook)
	{
		piece.AddMovementComponent(std::make_unique<StraightMovementComponent>(piece));
	}
	else if (type == chess::PieceType::Bishop)
	{
		piece.AddMovementComponent(std::make_unique<DiagonalMovementComponent>(piece));
	}
	else if (type == chess::PieceType::Knight)
	{
		piece.AddMovementComponent(std::make_unique<LShapedMovementComponent>(piece));
	}
	else
	{
		auto movement_range = type == chess::PieceType::King ? 1 : MovementComponent::kUnlimitedMovementRange;

		piece.AddMovementComponent(std::make_unique<StraightMovementComponent>(piece, movement_range));
		piece.AddMovementComponent(std::make_unique<DiagonalMovementComponent>(piece, movement_range));
	}
}

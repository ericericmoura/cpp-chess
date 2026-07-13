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
#include "GeneratedBoardInfo.h"

chess::GeneratedBoardInfo chess::PieceFactory::GeneratePieces(const Board& board, PiecesMap& map) const noexcept
{
	GeneratedBoardInfo board_info{};
	for (auto i = 0; i < 16; i++)
	{
		auto index = static_cast<float>(i);
		auto grid_y = static_cast<unsigned int>(floor(index / board.GetConfig().board_size_.x));
		auto grid_x = i % board.GetConfig().board_size_.x;

		auto black_coords = sf::Vector2u({ grid_x, grid_y });
		auto white_coords = black_coords;
		white_coords.y += 6;

		auto black_piece = GeneratePiece(board, black_coords, chess::Team::Black);
		auto white_piece = GeneratePiece(board, white_coords, chess::Team::White);

		if (white_piece.GetPieceType() == PieceType::King)
		{
			board_info.white_king_coords_ = white_coords;
		}
		if (black_piece.GetPieceType() == PieceType::King)
		{
			board_info.black_king_coords_ = black_coords;
		}
		map.try_emplace(black_coords, std::move(black_piece));
		map.try_emplace(white_coords, std::move(white_piece));
	}
	return board_info;
}

chess::PieceType chess::PieceFactory::GetPieceType(const sf::Vector2u& coords) const noexcept
{
	if (coords.y == 1 || coords.y == 6)
	{
		return PieceType::Pawn;
	}
	if (coords.x == 0 || coords.x == 7)
	{
		return PieceType::Rook;
	}
	if (coords.x == 1 || coords.x == 6)
	{
		return PieceType::Knight;
	}
	if (coords.x == 2 || coords.x == 5)
	{
		return PieceType::Bishop;
	}
	if (coords.x == 2 || coords.x == 5)
	{
		return PieceType::Bishop;
	}
	if (coords.x == 3)
	{
		return PieceType::Queen;
	}
	return PieceType::King;
}

chess::Piece chess::PieceFactory::GeneratePiece(const Board& board, const sf::Vector2u& coords, chess::Team team) const noexcept
{
	// Get type
	auto type = GetPieceType(coords);
	
	// Get image extension
	auto& team_name = team == chess::Team::White ? pieces_info.white_team_name_ : pieces_info.black_team_name_;
	auto piece_path = pieces_info.file_path_ + team_name + pieces_info.piece_name_map_.at(type) + pieces_info.sprite_extension_;
	auto piece = chess::Piece(team, piece_path);

	piece.SetCoordinates(board, coords);
	
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

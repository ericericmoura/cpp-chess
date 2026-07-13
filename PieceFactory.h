#pragma once

#include <unordered_map>
#include <string>

#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Vector2Hash.h"
#include "PieceType.h"
#include "Team.h"
#include "GeneratedBoardInfo.h"

namespace chess
{

class Board;

struct PiecesInformation
{
	std::string file_path_        = "graphics/";
	std::string sprite_extension_ = ".png";
	std::string black_team_name_  = "black_";
	std::string white_team_name_  = "white_";

	std::unordered_map<PieceType, std::string> piece_name_map_ =
	{
		{PieceType::Bishop, "bishop"},
		{PieceType::Knight, "knight"},
		{PieceType::Pawn  , "pawn"  },
		{PieceType::Rook  , "rook"  },
		{PieceType::Queen , "queen" },
		{PieceType::King  , "king"  },
	};
};

using PiecesMap = std::unordered_map<sf::Vector2u, Piece, Vec2uHash>;

class PieceFactory
{
public:
	GeneratedBoardInfo GeneratePieces(const Board& board, PiecesMap& map) const noexcept;

private:
	PiecesInformation pieces_info{};

	PieceType GetPieceType(const sf::Vector2u& coords) const noexcept;
	Piece GeneratePiece(const Board& board, const sf::Vector2u& coords, chess::Team team) const noexcept;
	void AttachComponentsForPiece(Piece& piece, chess::Team team, chess::PieceType type) const noexcept;
};

} // namespace chess
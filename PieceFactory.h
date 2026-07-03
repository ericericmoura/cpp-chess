#pragma once

#include <unordered_map>
#include <string>

#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Vector2Hash.h"
#include "PieceType.h"
#include "Team.h"

namespace chess
{

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
	void GeneratePieces(PiecesMap& board) const noexcept;

private:
	PiecesInformation pieces_info{};

	PieceType GetPieceType(const sf::Vector2u& position) const noexcept;
	Piece GeneratePiece(const sf::Vector2u& position, chess::Team team) const noexcept;
};

}
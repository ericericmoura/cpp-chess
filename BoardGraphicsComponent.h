#pragma once

#include <unordered_map>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Vector2Hash.h"

namespace chess
{

class Board;

using PiecesMap = std::unordered_map<sf::Vector2u, Piece, Vec2uHash>;

class BoardGraphicsComponent
{
public:
	void UpdateCapturedPiecesPosition(
		const Board& board,
		std::vector<Piece>& inactive_black_pieces,
		std::vector<Piece>& inactive_white_pieces) const noexcept;

	void DrawPieces(
		const PiecesMap& active_pieces_map, 
		const std::vector<Piece>& inactive_black_pieces,
		const std::vector<Piece>& inactive_white_pieces,
		sf::RenderTarget& target, 
		sf::RenderStates states) const;
};

} // namespace chess
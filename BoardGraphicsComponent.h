#pragma once

#include <unordered_map>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Vector2Hash.h"
#include "BoardConfiguration.h"

namespace chess
{

class Board;

using PiecesMap = std::unordered_map<sf::Vector2u, Piece, Vec2uHash>;

class BoardGraphicsComponent
{
public:
	void Initialize(file_io::BoardConfiguration& config);

	void UpdateCapturedPiecesPosition(
		const Board& board,
		std::vector<Piece>& inactive_black_pieces,
		std::vector<Piece>& inactive_white_pieces) noexcept;

	void UpdateSelectedCellPosition(
		const Board& board,
		sf::Vector2u coords) noexcept;

	void Update(const Board& board) noexcept;

	void DrawPieces(
		const PiecesMap& active_pieces_map, 
		const std::vector<Piece>& inactive_black_pieces,
		const std::vector<Piece>& inactive_white_pieces,
		sf::RenderTarget& target, 
		sf::RenderStates states) const;

private:
	sf::RectangleShape selected_cell_indicator_      {};
	sf::RectangleShape mouse_hovering_cell_indicator_{};

	void RepositionPieces(const Board& board, std::vector<Piece>& pieces, const sf::Vector2f& offset) noexcept;
};

} // namespace chess
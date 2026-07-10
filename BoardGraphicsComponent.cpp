#include "BoardGraphicsComponent.h"

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Board.h"
#include "Vector2Operators.h"

void chess::BoardGraphicsComponent::UpdateCapturedPiecesPosition(const Board& board, std::vector<Piece>& inactive_black_pieces, std::vector<Piece>& inactive_white_pieces) noexcept
{
	auto board_size_pixels = board.GetConfig().board_size_ * board.GetConfig().cell_size_;

	sf::Vector2f white_pieces_offset;
	white_pieces_offset.x = board.GetConfig().margin_.x + board_size_pixels.x + board.GetConfig().cell_size_.x;
	white_pieces_offset.y = board.GetConfig().margin_.y;
	RepositionPieces(board, inactive_white_pieces, white_pieces_offset);

	sf::Vector2f black_pieces_offset;
	black_pieces_offset.x = 0;
	black_pieces_offset.y = board.GetConfig().margin_.y;
	RepositionPieces(board, inactive_black_pieces, black_pieces_offset);
}

void chess::BoardGraphicsComponent::DrawPieces(
	const PiecesMap& active_pieces_map, 
	const std::vector<Piece>& inactive_black_pieces, 
	const std::vector<Piece>& inactive_white_pieces,
	sf::RenderTarget& target, 
	sf::RenderStates states) const
{
	for (auto& [position, piece] : active_pieces_map)
	{
		target.draw(piece, states);
	}
	for (auto& piece : inactive_black_pieces)
	{
		target.draw(piece, states);
	}
	for (auto& piece : inactive_white_pieces)
	{
		target.draw(piece, states);
	}
}

void chess::BoardGraphicsComponent::RepositionPieces(const Board& board, std::vector<Piece>& pieces, const sf::Vector2f& offset) noexcept
{
	auto max_col = 2;
	auto max_row = 8;

	auto board_size_pixels = board.GetConfig().board_size_ * board.GetConfig().cell_size_;

	for (auto i = 0; i < pieces.size(); ++i)
	{
		auto& piece = pieces.at(i);

		auto col = i % max_col;
		auto row = i / max_col;

		sf::Vector2f piece_pos{};
		piece_pos.x = offset.x + board.GetConfig().cell_size_.x * col;
		piece_pos.y = offset.y + board.GetConfig().cell_size_.y * row;

		piece.setPosition(piece_pos);
	}
}

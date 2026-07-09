#include "BoardGraphicsComponent.h"

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Board.h"

void chess::BoardGraphicsComponent::UpdateCapturedPiecesPosition(const Board& board, std::vector<Piece>& inactive_black_pieces, std::vector<Piece>& inactive_white_pieces) const noexcept
{
	auto max_col = 2;
	auto max_row = 8;

	for (auto i = 0; i < inactive_white_pieces.size(); ++i)
	{
		auto& piece = inactive_white_pieces.at(i);

		auto col = i % max_col;
		auto row = i / max_col;

		sf::Vector2f piece_pos{};
		piece_pos.x = board.GetConfig().margin_.x + board.GetConfig().board_size_.x + static_cast<float>(board.GetConfig().cell_size_.x) * col;
		piece_pos.y = board.GetConfig().margin_.y + static_cast<float>(board.GetConfig().cell_size_.y) * row;

		piece.setPosition(piece_pos);
	}

	for (auto i = 0; i < inactive_black_pieces.size(); ++i)
	{
		auto& piece = inactive_black_pieces.at(i);

		auto col = i % max_col;
		auto row = i / max_col;

		sf::Vector2f piece_pos{};
		piece_pos.x = board.GetConfig().margin_.x + static_cast<float>(board.GetConfig().cell_size_.x) * col;
		piece_pos.y = board.GetConfig().margin_.y + static_cast<float>(board.GetConfig().cell_size_.y) * row;

		piece.setPosition(piece_pos);
	}
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
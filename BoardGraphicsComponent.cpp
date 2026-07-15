#include "BoardGraphicsComponent.h"

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Board.h"
#include "Vector2Operators.h"
#include "Engine.h"
#include "BoardConfiguration.h"

void chess::BoardGraphicsComponent::Initialize(file_io::BoardConfiguration& config)
{
	selected_cell_indicator_.setSize(sf::Vector2f(config.cell_size_));
	selected_cell_indicator_.setFillColor(sf::Color(200.f, 0.f, 0.f, 255/2.f));

	mouse_hovering_cell_indicator_.setSize(sf::Vector2f(config.cell_size_));
	mouse_hovering_cell_indicator_.setFillColor(sf::Color(0.f, 0.f, 200.f, 255/2.f));
}

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

void chess::BoardGraphicsComponent::UpdateSelectedCellPosition(const Board& board, sf::Vector2u coords) noexcept
{
	selected_cell_indicator_.setPosition(board.GetPositionFromCoordinates(coords));
}

void chess::BoardGraphicsComponent::Update(const Board& board) noexcept
{
	mouse_hovering_cell_indicator_.setPosition(board.GetPositionFromCoordinates(board.GetCoordinatesFromPosition(Engine::local_mouse_position_)));
}

void chess::BoardGraphicsComponent::DrawPieces(
	const PiecesMap& active_pieces_map, 
	const std::vector<Piece>& inactive_black_pieces, 
	const std::vector<Piece>& inactive_white_pieces,
	sf::RenderTarget& target, 
	sf::RenderStates states) const
{
	for (auto& [coords, piece] : active_pieces_map)
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

	target.draw(selected_cell_indicator_, states);
	target.draw(mouse_hovering_cell_indicator_, states);
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

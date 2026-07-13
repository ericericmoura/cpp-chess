#include "Board.h"

#include <utility>
#include <algorithm>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Vector2Operators.h"
#include "BoardConfiguration.h"
#include "Team.h"
#include "PieceType.h"

chess::Board::Board(file_io::BoardConfiguration board_config) noexcept
	: board_config_{board_config}
	, graphics_    {board_config.texture_key_} 
{}

void chess::Board::GeneratePieces() noexcept
{
	auto info = factory_.GeneratePieces(*this, active_pieces_);
	white_king_coords_ = info.white_king_coords_;
	black_king_coords_ = info.black_king_coords_;
}

void chess::Board::SelectCoordinates(sf::Vector2u coords) noexcept
{
	auto it = active_pieces_.find(coords);
	if (it == active_pieces_.end())
	{
		return;
	}
	selected_coordinate_ = coords;
}

void chess::Board::MoveSelectedPieceToCoordinates(sf::Vector2u target_coords) noexcept
{	
	bool invalid_selected_coords_ = !selected_coordinate_.has_value() 
									|| target_coords == selected_coordinate_
									|| !IsCoordinatesWithinBounds(target_coords);
	if (invalid_selected_coords_)
	{
		return;
	}	
	auto it = active_pieces_.find(*selected_coordinate_);
	auto previous_coords = selected_coordinate_.value();
	selected_coordinate_ = {};

	auto piece_team = it->second.GetTeam();
	bool invalid_piece = it == active_pieces_.end() || it->second.GetTeam() != team_to_play_;
	if (invalid_piece || !it->second.CanMoveTo(*this, target_coords))
	{
		return;
	}
	CaptureAtCoordinates(target_coords);

	it->second.SetCoordinates(*this, target_coords);
	it->second.Moved(*this, previous_coords);

	auto piece_type = it->second.GetPieceType();

	auto node  = active_pieces_.extract(it);
	node.key() = target_coords;
	active_pieces_.insert(std::move(node));

	team_to_play_ = team_to_play_ == Team::White ? Team::Black : Team::White;

	if (piece_type != PieceType::King)
	{
		return;
	}
	if (piece_team == Team::Black)
	{
		black_king_coords_ = target_coords;
		return;
	}
	white_king_coords_ = target_coords;
}

void chess::Board::CaptureAtCoordinates(sf::Vector2u coords) noexcept
{
	auto it_target = active_pieces_.find(coords);
	if (it_target == active_pieces_.end())
	{
		return;
	}
	if (it_target->second.GetTeam() == chess::Team::White)
	{
		inactive_white_pieces_.emplace_back(std::move(it_target->second));
	}
	else
	{
		inactive_black_pieces_.emplace_back(std::move(it_target->second));
	}
	active_pieces_.erase(coords);

	board_graphics_.UpdateCapturedPiecesPosition(*this, inactive_black_pieces_, inactive_white_pieces_);
}

const chess::Piece* chess::Board::GetPieceAtCoordinates(sf::Vector2u coords) const noexcept
{
	auto it = active_pieces_.find(coords);
	if (it == active_pieces_.end())
	{
		return nullptr;
	}
	return &it->second;
}

bool chess::Board::IsCoordinatesOccupied(const sf::Vector2u& coords) const noexcept
{
	return active_pieces_.contains(coords);
}

void chess::Board::SetConfig(file_io::BoardConfiguration config) noexcept
{
	board_config_ = config;	
	graphics_.SetTextureKey(config.texture_key_);
}

const chess::file_io::BoardConfiguration& chess::Board::GetConfig() const noexcept
{
	return board_config_;
}

sf::Vector2f chess::Board::GetPositionFromCoordinates(const sf::Vector2u& coords) const noexcept
{
	auto position = coords * board_config_.cell_size_ + board_config_.margin_;
	return sf::Vector2f(position);
}

sf::Vector2u chess::Board::GetCoordinatesFromPosition(const sf::Vector2f& position) const noexcept
{
	auto local = position - sf::Vector2f(board_config_.margin_);

	local.x = std::max(local.x, 0.f);
	local.y = std::max(local.y, 0.f);

	return (sf::Vector2u(local) / board_config_.cell_size_);
}

bool chess::Board::IsCoordinatesWithinBounds(const sf::Vector2u& coords) const noexcept
{	
	return coords >= sf::Vector2u({0, 0}) && coords < board_config_.board_size_;
}

void chess::Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(graphics_, states);

	for (auto& [coords, piece] : active_pieces_)
	{
		target.draw(piece);
	}
	board_graphics_.DrawPieces(active_pieces_, inactive_black_pieces_, inactive_white_pieces_, target, states);
}
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

chess::Board::Board(BoardConfiguration board_config) noexcept
	: board_config_{board_config}
	, graphics_    {board_config.texture_key_} 
{}

void chess::Board::GeneratePieces() noexcept
{
	factory_.GeneratePieces(*this, active_pieces_);
}

void chess::Board::SelectCoordinate(sf::Vector2u coord) noexcept
{
	auto it = active_pieces_.find(coord);
	if (it == active_pieces_.end())
	{
		return;
	}
	selected_coordinate_ = coord;
}

void chess::Board::MoveSelectedPieceToCoordinate(sf::Vector2u coord) noexcept
{	
	if (!selected_coordinate_.has_value() 
		|| coord == selected_coordinate_
		|| !IsCoordinatesWithinBounds(coord))
	{
		return;
	}	
	auto it = active_pieces_.find(*selected_coordinate_);
	selected_coordinate_ = {};
	if (it == active_pieces_.end())
	{
		return;
	}
	if (!it->second.TryMove(*this, coord))
	{		
		return;
	}
	CaptureAtCoordinate(coord);

	it->second.SetBoardPosition(*this, coord);
	auto node = active_pieces_.extract(it);
	node.key() = coord;
	active_pieces_.insert(std::move(node));
}

void chess::Board::CaptureAtCoordinate(sf::Vector2u coord) noexcept
{
	auto it_target = active_pieces_.find(coord);
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
	active_pieces_.erase(coord);

	board_graphics_.UpdateCapturedPiecesPosition(*this, inactive_black_pieces_, inactive_white_pieces_);
}

const chess::Piece* chess::Board::GetPieceAtCoordinate(sf::Vector2u coord) const noexcept
{
	auto it = active_pieces_.find(coord);
	if (it == active_pieces_.end())
	{
		return nullptr;
	}
	return &it->second;
}

bool chess::Board::IsCoordinateOccupied(const sf::Vector2u& coord) const noexcept
{
	return active_pieces_.contains(coord);
}

const BoardConfiguration& chess::Board::GetConfig() const noexcept
{
	return board_config_;
}

sf::Vector2f chess::Board::GetPositionFromCoordinates(const sf::Vector2u& coordinates) const noexcept
{
	auto position = coordinates * board_config_.cell_size_ + board_config_.margin_;
	return sf::Vector2f(position);
}

sf::Vector2u chess::Board::GetCoordinatesFromPosition(const sf::Vector2f& position) const noexcept
{
	auto local = position - sf::Vector2f(board_config_.margin_);

	local.x = std::max(local.x, 0.f);
	local.y = std::max(local.y, 0.f);

	return (sf::Vector2u(local) / board_config_.cell_size_);
}

bool chess::Board::IsCoordinatesWithinBounds(const sf::Vector2u& position) const noexcept
{	
	return position >= sf::Vector2u({0, 0}) && position < board_config_.board_size_;
}

void chess::Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(graphics_, states);

	for (auto& [position, piece] : active_pieces_)
	{
		target.draw(piece);
	}
	board_graphics_.DrawPieces(active_pieces_, inactive_black_pieces_, inactive_white_pieces_, target, states);
}
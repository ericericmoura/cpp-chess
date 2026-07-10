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

void chess::Board::SelectCoordinates(sf::Vector2u coords) noexcept
{
	auto it = active_pieces_.find(coords);
	if (it == active_pieces_.end())
	{
		return;
	}
	selected_coordinate_ = coords;
}

void chess::Board::MoveSelectedPieceToCoordinates(sf::Vector2u coords) noexcept
{	
	if (!selected_coordinate_.has_value() 
		|| coords == selected_coordinate_
		|| !IsCoordinatesWithinBounds(coords))
	{
		return;
	}	
	auto it = active_pieces_.find(*selected_coordinate_);
	selected_coordinate_ = {};
	if (it == active_pieces_.end())
	{
		return;
	}
	if (!it->second.TryMove(*this, coords))
	{		
		return;
	}
	CaptureAtCoordinates(coords);

	it->second.SetBoardPosition(*this, coords);
	auto node = active_pieces_.extract(it);
	node.key() = coords;
	active_pieces_.insert(std::move(node));
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

void chess::Board::SetConfig(BoardConfiguration config) noexcept
{
	board_config_ = config;	
	graphics_.SetTextureKey(config.texture_key_);
}

const BoardConfiguration& chess::Board::GetConfig() const noexcept
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
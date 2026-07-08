#include "Board.h"

#include <utility>
#include <algorithm>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Vector2Operators.h"
#include "BoardConfiguration.h"

chess::Board::Board(BoardConfiguration board_config)
	: board_config_{board_config}
	, graphics_    {board_config_.texture_key_}
{}

void chess::Board::GeneratePieces() noexcept
{
	factory_.GeneratePieces(active_pieces_);
}

void chess::Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(graphics_, states);

	for (auto& [position, piece] : active_pieces_)
	{
		target.draw(piece);
	}

	for (auto& piece : inactive_pieces_)
	{
		target.draw(piece);
	}
}

void chess::Board::SelectPosition(sf::Vector2u position) noexcept
{
	auto it = active_pieces_.find(position);
	if (it == active_pieces_.end())
	{
		position_selected_ = false;
		return;
	}
	selected_position_ = position;	
	position_selected_ = true;
}

void chess::Board::TryMoveTo(sf::Vector2u target) noexcept
{
	position_selected_ = false;
	if (!IsCoordinatesValid(target))
	{
		return;
	}
	auto it = active_pieces_.find(selected_position_);
	if (it == active_pieces_.end())
	{
		return;
	}
	if (!it->second.TryMove(*this, target))
	{		
		return;
	}

	CaptureAt(target);

	it->second.SetBoardPosition(target);
	auto node = active_pieces_.extract(it);
	node.key() = target;
	active_pieces_.insert(std::move(node));
}

void chess::Board::CaptureAt(sf::Vector2u position) noexcept
{
	auto it_target = active_pieces_.find(position);
	if (it_target == active_pieces_.end())
	{
		return;
	}
	it_target->second.setPosition({ kMargin.x + static_cast<float>(kCellSize.x) * inactive_pieces_.size(), 10 });
	inactive_pieces_.emplace_back(std::move(it_target->second));
	active_pieces_.erase(position);
}

const chess::Piece* chess::Board::GetPieceAt(sf::Vector2u position) const noexcept
{
	auto it = active_pieces_.find(position);
	if (it == active_pieces_.end())
	{
		return nullptr;
	}
	return &it->second;
}

bool chess::Board::IsPositionOccupied(const sf::Vector2u& position) const noexcept
{
	return active_pieces_.contains(position);
}

sf::Vector2f chess::Board::GetPositionInPixels(sf::Vector2u board_position) noexcept
{
	auto margin   = chess::Board::kMargin;
	auto position = board_position * chess::Board::kCellSize + margin;
	return sf::Vector2f(position);
}

sf::Vector2u chess::Board::GetCoordinates(sf::Vector2f position) noexcept
{
	auto margin = chess::Board::kMargin;
	sf::Vector2f local = position - sf::Vector2f(margin);

	local.x = std::max(local.x, 0.f);
	local.y = std::max(local.y, 0.f);

	return (sf::Vector2u(local) / chess::Board::kCellSize);
}

bool chess::Board::IsCoordinatesValid(sf::Vector2u position) noexcept
{
	return position >= sf::Vector2u({0, 0}) && position < kBoardSize;
}

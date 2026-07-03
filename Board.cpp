#include "Board.h"

#include <memory>
#include <utility>
#include <cmath>
#include <algorithm>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Vector2Operators.h"
#include "Team.h"
#include "StraightMovementComponent.h"

chess::Board::Board()
	: graphics_{kTextureKey}
{
}

void chess::Board::GeneratePieces() noexcept
{
	for (auto i = 0; i < 16; i++)
	{
		auto index  = static_cast<float>(i);
		auto grid_y = static_cast<unsigned int>(floor(index / kBoardSize.x));
		auto grid_x = i % kBoardSize.x;

		auto position = sf::Vector2u({ grid_x, grid_y });

		auto black_piece = chess::Piece(chess::Team::Black, "graphics/black_rook.png", position);
		black_piece.AddMovementComponent(std::make_unique<StraightMovementComponent>(chess::Team::Black));
		active_pieces_.try_emplace(position, std::move(black_piece));

		auto white_grid_y   = grid_y + 6;
		auto white_position = sf::Vector2u({ grid_x, white_grid_y });

		auto white_piece = chess::Piece(chess::Team::White, "graphics/white_rook.png", white_position);
		white_piece.AddMovementComponent(std::make_unique<StraightMovementComponent>(chess::Team::White));
		active_pieces_.try_emplace(white_position, std::move(white_piece));
	}
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
	if (!it->second.TryMove(active_pieces_, target))
	{		
		return;
	}
	auto it_target = active_pieces_.find(target);
	if (it_target != active_pieces_.end())
	{
		it_target->second.setPosition({ kMargin.x + static_cast<float>(kCellSize.x) * inactive_pieces_.size(), 10});
		inactive_pieces_.emplace_back(std::move(it_target->second));
		active_pieces_.erase(target);
	}

	it->second.SetBoardPosition(target);
	auto node = active_pieces_.extract(it);
	node.key() = target;
	active_pieces_.insert(std::move(node));
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

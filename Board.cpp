#include "Board.h"

#include <cmath>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Vector2Operators.h"
#include "Team.h"
#include "StraightMovementComponent.h"
#include <memory>
#include <utility>

chess::Board::Board()
	: graphics_{kTextureKey}
{
}

void chess::Board::GeneratePieces() noexcept
{
	for (auto i = 0; i < 16; i++)
	{
		auto index  = static_cast<float>(i + 1);
		auto grid_y = static_cast<unsigned int>(ceil(index / kBoardSize.x));
		auto grid_x = i % kBoardSize.x + 1;

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
}

void chess::Board::SelectPosition(sf::Vector2u position) noexcept
{
	selected_position_ = position;	
}

void chess::Board::TryMoveTo(sf::Vector2u target) noexcept
{
	auto it = active_pieces_.find(selected_position_);
	if (it == active_pieces_.end())
	{
		return;
	}
	if (it->second.TryMove(active_pieces_, target))
	{
		auto node = active_pieces_.extract(it);
		node.mapped().SetBoardPosition(target);
		node.key() = target;
		active_pieces_.insert(std::move(node));
	};
}

sf::Vector2f chess::Board::GetPositionInPixels(sf::Vector2u board_position) noexcept
{
	auto margin   = chess::Board::GetBoardMargin();
	auto position = board_position * chess::Board::kCellSize + margin;
	return sf::Vector2f(position);
}

sf::Vector2u chess::Board::GetBoardCoordinates(sf::Vector2f position) noexcept
{
	auto margin = chess::Board::GetBoardMargin();
	return (sf::Vector2u(position) - margin) / chess::Board::kCellSize;
}

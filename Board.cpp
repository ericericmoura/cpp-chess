#include "Board.h"

#include <cmath>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Piece.h"

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
		board_.try_emplace(position, chess::Piece("graphics/black_pawn.png", position));

		auto white_grid_y   = grid_y + 6;
		auto white_position = sf::Vector2u({ grid_x, white_grid_y });
		board_.try_emplace(white_position, chess::Piece("graphics/white_pawn.png", white_position));
	}
}

void chess::Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(graphics_, states);

	states = sf::RenderStates();
	for (auto& [position, piece] : board_)
	{
		target.draw(piece, states);
	}
}

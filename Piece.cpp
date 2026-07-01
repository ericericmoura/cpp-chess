#include "Piece.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "Vector2Operators.h"
#include "Board.h"

chess::Piece::Piece(sf::Vector2i starting_pos) noexcept
{}

void chess::Piece::SetBoardPosition(sf::Vector2i pos) noexcept
{
	if (pos > chess::Board::kBoardSize ||
		pos < sf::Vector2i({0, 0}))
	{
		return;
	}
	setPosition(sf::Vector2f(pos - sf::Vector2i{1, 1}));
}

void chess::Piece::draw(sf::RenderTarget& target, sf::RenderStates states) const
{		
	auto position = getPosition() * sf::Vector2f(chess::Board::kCellSize);
	states.transform.translate(position).rotate(getRotation()).scale(getScale());
	graphics_.draw(target, states);
}

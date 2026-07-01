#include "Piece.h"

#include <string>
#include <utility>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "Vector2Operators.h"
#include "Board.h"
#include "Team.h"

chess::Piece::Piece(Team team, const std::string& texture_key, sf::Vector2u starting_pos) noexcept
	: graphics_(texture_key)
{
	SetBoardPosition(std::move(starting_pos));

	graphics_.CenterOrigin(true);

	team_ = team;
}

void chess::Piece::SetBoardPosition(sf::Vector2u pos) noexcept
{
	if (pos > chess::Board::kBoardSize ||
		pos < sf::Vector2u({1, 1}))
	{
		return;
	}
	setPosition(sf::Vector2f(pos - sf::Vector2u{1, 1}));
}

void chess::Piece::draw(sf::RenderTarget& target, sf::RenderStates states) const
{		
	auto texture_size = graphics_.GetTextureSize();
	auto offset   = sf::Vector2f(chess::Board::kMargin + chess::Board::kCellSize/2u);
	auto position = getPosition() * sf::Vector2f(chess::Board::kCellSize) + offset;
	states.transform.translate(position).rotate(getRotation()).scale(getScale());
	graphics_.draw(target, states);
}

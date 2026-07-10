#include "Piece.h"

#include <string>
#include <utility>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "Board.h"
#include "Team.h"
#include "MovementComponent.h"

chess::Piece::Piece(Team team, const std::string& texture_key) noexcept
	: graphics_(texture_key)
{
	graphics_.CenterOrigin(true);
	team_ = team;
}

void chess::Piece::AddMovementComponent(std::unique_ptr<MovementComponent> comp) noexcept
{
	if (!comp)
	{
		return;
	}
	movement_components_.emplace_back(std::move(comp));
}

bool chess::Piece::TryMove(Board& board, sf::Vector2u position) noexcept
{
	for (auto& comp : movement_components_)
	{
		auto success = comp->TryMove(board, board.GetCoordinatesFromPosition(getPosition()), position);
		if (success)
		{
			return true;
		}
	}
	return false;
}

void chess::Piece::SetBoardPosition(const Board& board, const sf::Vector2u& pos) noexcept
{
	if (!board.IsCoordinatesWithinBounds(pos))
	{
		return;
	}
	sf::Vector2f centered_position = board.GetPositionFromCoordinates(pos);
	centered_position += sf::Vector2f(board.GetConfig().cell_size_ / 2u);
	setPosition(centered_position);
}

void chess::Piece::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	graphics_.draw(target, states);
}
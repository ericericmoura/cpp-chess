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

bool chess::Piece::CanMoveTo(const Board& board, sf::Vector2u coords) const noexcept
{
	for (auto& comp : movement_components_)
	{
		auto success = comp->CanMoveTo(board, board.GetCoordinatesFromPosition(getPosition()), coords);
		if (success)
		{
			return true;
		}
	}
	return false;
}

void chess::Piece::Moved(Board& board, sf::Vector2u previous_coords) noexcept
{
	for (auto& comp : movement_components_)
	{
		comp->Moved(previous_coords, board.GetCoordinatesFromPosition(getPosition()), board);
	}
}

void chess::Piece::SetCoordinates(const Board& board, const sf::Vector2u& coords) noexcept
{
	if (!board.IsCoordinatesWithinBounds(coords))
	{
		return;
	}
	sf::Vector2f centered_position = board.GetPositionFromCoordinates(coords);
	centered_position += sf::Vector2f(board.GetConfig().cell_size_ / 2u);
	setPosition(centered_position);
}

void chess::Piece::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	graphics_.draw(target, states);
}
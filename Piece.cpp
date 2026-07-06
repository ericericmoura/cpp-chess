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

chess::Piece::Piece(Team team, const std::string& texture_key, sf::Vector2u starting_pos) noexcept
	: graphics_(texture_key)
{
	SetBoardPosition(std::move(starting_pos));

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
		auto success = comp->TryMove(board, chess::Board::GetCoordinates(getPosition()), position);
		if (success)
		{
			return true;
		}
	}
	return false;
}

void chess::Piece::SetBoardPosition(sf::Vector2u pos) noexcept
{
	if (!chess::Board::IsCoordinatesValid(pos))
	{
		return;
	}
	setPosition(chess::Board::GetPositionInPixels(pos) + sf::Vector2f(chess::Board::kCellSize / 2u));
}

void chess::Piece::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	graphics_.draw(target, states);
}
#pragma once

#include <unordered_map>
#include <functional>
#include <utility>

#include <SFML/System/Vector2.hpp>

#include "Team.h"
#include "Vector2Hash.h"

namespace chess {

class Piece;

using PiecesMap = std::unordered_map<sf::Vector2u, Piece, Vec2uHash>;

class MovementComponent
{
public:
	static inline constexpr unsigned int kUnlimitedMovementRange = 0;

	MovementComponent(Team team)
		: team_(team)
	{}

	virtual bool TryMove(
		Piece& piece, 
		const sf::Vector2u& current_pos, 
		const sf::Vector2u& target_pos, 
		PiecesMap& pieces) noexcept = 0;

	void SetObserver(std::function<void(sf::Vector2u old_position, sf::Vector2u new_position)>&& observer) noexcept
	{
		observer_ = std::move(observer);
	}

protected:
	Team team_{};

	virtual bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept = 0;
	virtual bool IsPositionBlocked(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, PiecesMap& pieces) const noexcept = 0;

private:
	std::function<void(sf::Vector2u old_position, sf::Vector2u new_position)> observer_{};

	void Notify(sf::Vector2u old_position, sf::Vector2u new_position) const noexcept
	{
		if (observer_)
		{
			observer_(old_position, new_position);
		}
	};
};

}
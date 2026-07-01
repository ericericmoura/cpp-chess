#pragma once

#include <unordered_map>
#include <functional>
#include <utility>

#include <SFML/System/Vector2.hpp>

#include "Piece.h"
#include "Team.h"

namespace chess {

class MovementComponent
{
public:
	MovementComponent(Team team)
		: team_(team)
	{}

	virtual bool TryMove(
		const Piece& piece, 
		const sf::Vector2i& current_pos, 
		const sf::Vector2i& target_pos, 
		std::unordered_map<sf::Vector2i, Piece>& pieces) const noexcept = 0;
	
	void SetObserver(std::function<void(sf::Vector2i old_position, sf::Vector2i new_position)>&& observer) noexcept
	{
		observer_ = std::move(observer);
	}

protected:
	Team team_{};

	virtual bool IsPositionReachable(const sf::Vector2i& current_pos, const sf::Vector2i& target_pos) const noexcept = 0;
	virtual bool IsPositionBlocked  (const sf::Vector2i& current_pos, const sf::Vector2i& target_pos, std::unordered_map<sf::Vector2i, Piece>& pieces) const noexcept = 0;

private:
	std::function<void(sf::Vector2i old_position, sf::Vector2i new_position)> observer_{};

	void Notify(sf::Vector2i old_position, sf::Vector2i new_position) const noexcept
	{
		if (observer_)
		{
			observer_(old_position, new_position);
		}
	};
};

}
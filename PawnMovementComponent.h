#pragma once

#include <SFML/System/Vector2.hpp>

#include "StraightMovementComponent.h"
#include "Team.h"

namespace chess
{

class PawnMovementComponent : public StraightMovementComponent
{
public:
	PawnMovementComponent(Team team)
		: StraightMovementComponent(team, 2)
	{};
	
	bool IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept override;
	virtual void Moved() noexcept override;

private:
	bool first_movement_ = true;
};

}

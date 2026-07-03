#include "PawnMovementComponent.h"

#include <cmath>

#include <SFML/System/Vector2.hpp>

#include "StraightMovementComponent.h"

bool chess::PawnMovementComponent::IsPositionReachable(const sf::Vector2u& current_pos, const sf::Vector2u& target_pos, bool occupied_by_enemy) const noexcept
{
    if (!occupied_by_enemy)
    {
        return StraightMovementComponent::IsPositionReachable(current_pos, target_pos, occupied_by_enemy);
    }
    auto distance_x = static_cast<int>(target_pos.x - current_pos.x);
    auto distance_y = static_cast<int>(target_pos.y - current_pos.y);

    return abs(distance_x) == abs(distance_y);
}

void chess::PawnMovementComponent::Moved() noexcept
{
    if (!first_movement_)
    {
        return;
    }
    first_movement_ = false;
    movement_range_--;
}

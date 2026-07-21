#pragma once

#include <functional>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include "Container.h"
#include "../Subject.h"

namespace chess
{
namespace ui
{

class Button : public Container
{
public:
	unsigned int OnClicked(std::function<void()> observer);
	void RemoveOnClicked(unsigned int id);

	// Inherited via Container
	virtual void HandleInput(sf::Vector2u mouse_pos) override;

private:
	Subject<> clicked_{};

	bool  can_click_ = true;
	float click_delay_ = .2f;

	sf::Time time_since_last_click_{};
};

} // namespace ui
} // namespace chess
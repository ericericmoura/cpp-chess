#pragma once

#include <functional>

#include <SFML/System/Vector2.hpp>

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

	// Inherited via Element
	virtual void HandleInput(sf::Vector2u mouse_pos) override;

private:
	Subject<> clicked_{};
};

} // namespace ui
} // namespace chess
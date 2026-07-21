#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "Container.h"

namespace chess
{
namespace ui
{

class ColorRect : public Container
{
public:
	ColorRect() = default;
	ColorRect(sf::RectangleShape&& shape);

	// Inherited via Container
	virtual void Update(sf::Vector2u window_size) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::RectangleShape shape_{};
};

} // namespace ui
} // namespace chess
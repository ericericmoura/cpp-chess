#include "UI/ColorRect.h"

#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "UI/Container.h"

chess::ui::ColorRect::ColorRect(sf::RectangleShape&& shape)
{
	shape_ = std::move(shape);
}

void chess::ui::ColorRect::Update(sf::Vector2u window_size)
{
	Container::Update(window_size);

	shape_.setSize    (sf::Vector2f(GetSize() + padding_));
	shape_.setPosition(sf::Vector2f(GetPosition() + padding_/static_cast<unsigned int>(2)));
}

void chess::ui::ColorRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape_);
	
	Container::draw(target, states);
}

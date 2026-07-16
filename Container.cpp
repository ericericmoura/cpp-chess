#include "UI/Container.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "UI/Element.h"

void chess::ui::Container::Update(sf::Vector2u window_size)
{
	Element::Update(window_size);

	// ALIGN EACH ELEMENT WITH THE CONTAINER
	for (auto& element : elements_)
	{
	}
}

void chess::ui::Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= GetTransform();
	for (auto& element : elements_)
	{
		target.draw(element, states);
	}
}

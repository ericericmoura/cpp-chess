#include "StandardGraphicsComponent.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include "Board.h"

void chess::StandardGraphicsComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::CircleShape circle{};
	circle.setRadius(Board::kCellSize.x/2.f);
	circle.setFillColor(sf::Color::White);

	target.draw(circle, states);
}

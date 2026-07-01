#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace chess {

class StandardGraphicsComponent : public sf::Drawable
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:	
};

}
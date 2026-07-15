#pragma once

#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Element.h"

namespace chess
{
namespace ui
{

class Container : Element
{
public:	
	virtual void Update(sf::Vector2u window_size) override;

	// Inherited via sf::Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<Element> elements_{};	

	sf::Vector2u grid_   {};
	sf::Vector2u margin_ {};
	sf::Vector2u padding_{};
	sf::Vector2u spacing_{};
};

} // namespace ui
} // namespace chess
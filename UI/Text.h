#pragma once

#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Element.h"

namespace chess
{
namespace ui
{

class Text : public Element
{
public:
	Text(std::string font_key);
	// Inherited via Element
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::string font_key_ = "";
	sf::Text text_;
};

} // namespace ui
} // namespace chess
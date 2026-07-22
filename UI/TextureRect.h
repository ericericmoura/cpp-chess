#pragma once

#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Element.h"

namespace chess
{
namespace ui
{

class TextureRect : public Element
{
public:
	TextureRect(std::string texture_key);	

	// Inherited via Element
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::string texture_key_{};
};

} // namespace ui
} // namespace chess
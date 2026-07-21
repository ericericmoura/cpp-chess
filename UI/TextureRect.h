#pragma once

#include <string_view>

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
	TextureRect(std::string_view texture_key);	

	// Inherited via sf::Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::string_view texture_key_{};

	bool tiled_ = false;
};

} // namespace ui
} // namespace chess
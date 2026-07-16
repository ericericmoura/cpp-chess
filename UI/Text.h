#pragma once

#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string_view>

#include "Element.h"

namespace chess
{
namespace ui
{

class Text : public Element
{
public:
	Text(std::string_view font_key);
	void SetFontKey(std::string_view font_key_) noexcept;
	
	sf::Text& GetText() noexcept;

	// Inherited via Element
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Text text_;
};

} // namespace ui
} // namespace chess
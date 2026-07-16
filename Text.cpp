#include "UI/Text.h"

#include <string_view>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "UI/FontStore.h"
#include "UI/Element.h"

chess::ui::Text::Text(std::string_view font_key)
	: text_(FontStore::GetInstance().GetFont(font_key.data()))
{}

void chess::ui::Text::SetFontKey(std::string_view font_key) noexcept
{
	text_.setFont(FontStore::GetInstance().GetFont(font_key.data()));
}

sf::Text& chess::ui::Text::GetText() noexcept
{
	return text_;
}

void chess::ui::Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Element::draw(target, states);

	target.draw(text_);
}

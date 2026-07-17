#include "UI/Text.h"

#include <string_view>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "UI/FontStore.h"
#include "UI/Element.h"

chess::ui::Text::Text(std::string_view font_key)
	: text_(FontStore::GetInstance().GetFont(font_key.data()))
{
}

void chess::ui::Text::SetFontKey(std::string_view font_key) noexcept
{
	text_.setFont(FontStore::GetInstance().GetFont(font_key.data()));	
}

sf::Text& chess::ui::Text::GetText() noexcept
{
	return text_;
}

void chess::ui::Text::Update(sf::Vector2u window_size)
{
	Element::Update(window_size);

	SetSize(sf::Vector2u(text_.getGlobalBounds().size));
}

void chess::ui::Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= GetTransform();
	target.draw(text_, states);
}

#include "UI/TextureRect.h"

#include <string>
#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "BitmapStore.h"
#include "Vector2Operators.h"

chess::ui::TextureRect::TextureRect(std::string texture_key)
	: texture_key_(std::move(texture_key))
{}

void chess::ui::TextureRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= GetTransform();

	const auto& texture = BitmapStore::GetInstance().GetTexture(texture_key_.data());
	auto texture_size = texture.getSize();
	auto scale = sf::Vector2f(GetSize()) / sf::Vector2f(texture_size);

	sf::Sprite sprite{ texture };
	sprite.setScale(scale);
	target.draw(sprite, states);
}

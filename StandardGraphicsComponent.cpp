#include "StandardGraphicsComponent.h"

#include <string>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "BitmapStore.h"

chess::StandardGraphicsComponent::StandardGraphicsComponent(const std::string& texture_key) noexcept
	: texture_key_(texture_key)
{}

sf::Vector2u chess::StandardGraphicsComponent::GetTextureSize() const noexcept
{
	if (texture_key_.empty())
	{
		return {};
	}
	return BitmapStore::GetInstance().GetTexture(texture_key_).getSize();
}

void chess::StandardGraphicsComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (texture_key_.empty())
	{
		return;
	}
	sf::Sprite sprite{BitmapStore::GetInstance().GetTexture(texture_key_)};	
	if (center_origin_)
	{
		sprite.setOrigin(
			{
				BitmapStore::GetInstance().GetTexture(texture_key_).getSize().x / 2.f,
				BitmapStore::GetInstance().GetTexture(texture_key_).getSize().y / 2.f
			});
	}
	target.draw(sprite, states);
}

#pragma once

#include <string>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

namespace chess {

class StandardGraphicsComponent : public sf::Drawable
{
public:	
	StandardGraphicsComponent() = default;
	StandardGraphicsComponent(const std::string& texture_key) noexcept;

	sf::Vector2u GetTextureSize() const noexcept;
	
	void CenterOrigin(bool value) noexcept
	{
		center_origin_ = value;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::string texture_key_{};

	bool center_origin_ = false;
};

}
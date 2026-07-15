#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <optional>

namespace chess
{
namespace ui
{

class Element : public sf::Drawable, private sf::Transformable
{
public:
	void SetPosition(sf::Vector2f position_percentage) noexcept;
	void SetSize(sf::Vector2f size_percentage) noexcept;
	void SetSize(sf::Vector2u size) noexcept;
	
	sf::Vector2u GetPosition() const noexcept;
	sf::Vector2u GetSize    () const noexcept;

	void SetOrigin(sf::Vector2f origin) noexcept;

	virtual void Update(sf::Vector2u window_size);

	// Inherited via sf::Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Vector2f position_percentage_{ 1.f, 1.f };	
	
	std::optional<sf::Vector2f> size_percentage_{};

	sf::Vector2u size_{};
};

} // namespace ui
} // namespace chess
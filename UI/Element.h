#pragma once

#include <optional>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

namespace chess
{
namespace ui
{

class Element : public sf::Drawable, private sf::Transformable
{
public:
	void SetResponsivePosition(sf::Vector2f position_percentage) noexcept;
	void SetResponsiveSize    (sf::Vector2f size_percentage    ) noexcept;

	void DisableResponsivePosition() noexcept;
	void DisableResponsiveSize    () noexcept;
	
	void SetPosition(sf::Vector2u position) noexcept;
	void SetSize    (sf::Vector2u size    ) noexcept;
	
	sf::Vector2u GetPosition() const noexcept;
	sf::Vector2u GetSize    () const noexcept;

	const sf::Transform& GetTransform() const noexcept;

	void SetOrigin(sf::Vector2f origin) noexcept;

	virtual void Update(sf::Vector2u window_size);

	virtual void HandleInput(sf::Vector2u mouse_pos) 
	{};

	// Inherited via sf::Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

private:
	std::optional<sf::Vector2f> position_percentage_{};	
	std::optional<sf::Vector2f> size_percentage_    {};

	sf::Vector2u size_{};
};

} // namespace ui
} // namespace chess
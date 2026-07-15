#include "UI/Element.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Vector2Operators.h"

void chess::ui::Element::SetResponsivePosition(sf::Vector2f position_percentage) noexcept
{
	if (position_percentage.x > 1.f || position_percentage.x < 0.f ||
		position_percentage.y > 1.f || position_percentage.y < 0.f)
	{
		return;
	}
	position_percentage_ = position_percentage;
}

void chess::ui::Element::SetResponsiveSize(sf::Vector2f size_percentage) noexcept
{
	if (size_percentage.x > 1.f || size_percentage.x < 0.f ||
		size_percentage.y > 1.f || size_percentage.y < 0.f)
	{
		return;
	}
	size_percentage_ = size_percentage;
}

void chess::ui::Element::DisableResponsivePosition() noexcept
{
	position_percentage_ = {};
}

void chess::ui::Element::DisableResponsiveSize() noexcept
{
	size_percentage_ = {};
}

void chess::ui::Element::SetPosition(sf::Vector2u position) noexcept
{
	setPosition(sf::Vector2f(position));
}

void chess::ui::Element::SetSize(sf::Vector2u size) noexcept
{
	size_ = size;
}

sf::Vector2u chess::ui::Element::GetPosition() const noexcept
{
	return sf::Vector2u(getPosition());
}

sf::Vector2u chess::ui::Element::GetSize() const noexcept
{
	return size_;
}

void chess::ui::Element::SetOrigin(sf::Vector2f origin) noexcept
{
	setOrigin(origin);
}

void chess::ui::Element::Update(sf::Vector2u window_size)
{
	if (position_percentage_.has_value())
	{
		setPosition(position_percentage_.value() * sf::Vector2f(window_size));
	}
	if (size_percentage_.has_value())
	{
		SetSize(sf::Vector2u(size_percentage_.value() * sf::Vector2f(window_size)));
	}
}

void chess::ui::Element::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
}

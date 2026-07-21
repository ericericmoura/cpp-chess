#include "UI/Container.h"

#include <algorithm>
#include <utility>
#include <memory>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "UI/Element.h"

void chess::ui::Container::SetIsVertical(bool value) noexcept
{
	is_vertical_ = value;
}

void chess::ui::Container::SetSpacing(float value) noexcept
{
	spacing_ = value;
}

void chess::ui::Container::SetPadding(sf::Vector2u value) noexcept
{
	padding_ = std::move(value);
}

void chess::ui::Container::AddElement(std::unique_ptr<Element> element) noexcept
{
	elements_.emplace_back(std::move(element));
}

void chess::ui::Container::Update(sf::Vector2u window_size)
{
	Element::Update(window_size);

	sf::Vector2u element_position{};
	element_position = GetPosition();
	element_position += padding_;

	sf::Vector2u container_size_{};

	// ALIGN EACH ELEMENT WITH THE CONTAINER
	for (auto& element : elements_)
	{
		if (!element)
		{
			continue;
		}
		element->Update(window_size);
		element->SetPosition(element_position);
		if (is_vertical_)
		{
			element_position.y += spacing_ + element->GetSize().y;
			container_size_ .y += spacing_ + element->GetSize().y;
			container_size_ .x  = std::max(container_size_.x, element->GetSize().x);
		}
		else
		{
			element_position.x += spacing_ + element->GetSize().x;
			container_size_ .x += spacing_ + element->GetSize().x;
			container_size_ .y  = std::max(container_size_.y, element->GetSize().y);
		}
	}
	SetSize(std::move(container_size_));
}

void chess::ui::Container::HandleInput(sf::Vector2u mouse_pos)
{
	for (auto& element : elements_)
	{
		element->HandleInput(mouse_pos);
	}
}

void chess::ui::Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//states.transform *= GetTransform();
	for (auto& element : elements_)
	{
		target.draw(*element, states);
	}
}

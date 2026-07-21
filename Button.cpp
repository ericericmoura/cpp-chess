#include "UI/Button.h"

#include <iostream>
#include <functional>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Engine.h"

unsigned int chess::ui::Button::OnClicked(std::function<void()> observer)
{
	return clicked_.Subscribe(observer);
}

void chess::ui::Button::RemoveOnClicked(unsigned int id)
{
	clicked_.Remove(id);
}

void chess::ui::Button::HandleInput(sf::Vector2u mouse_pos)
{
	if (!can_click_)
	{
		can_click_ = (Engine::elapsed_time_ - time_since_last_click_).asSeconds() > click_delay_;
		return;
	}
	sf::Rect<unsigned int> bounds_{};
	bounds_.position = GetPosition();
	bounds_.size     = GetSize();
	
	auto is_hovering = bounds_.contains(mouse_pos);
	if (is_hovering && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		clicked_.Notify();

		can_click_ = false;
		time_since_last_click_ = Engine::elapsed_time_;
	}
}

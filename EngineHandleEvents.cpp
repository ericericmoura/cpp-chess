#include "Engine.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/WindowEnums.hpp>

void chess::Engine::HandleEvent(sf::Event event)
{
	if (event.is<sf::Event::Closed>())
	{
		window_.close();
	}
	else if (auto mouse = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mouse->button == sf::Mouse::Button::Left)
		{
			auto mouse_position = window_.mapPixelToCoords(mouse->position, main_camera_);
			auto board_mouse_position = chess_board_.GetCoordinatesFromPosition(mouse_position);

			if (!chess_board_.IsCoordinatesSelected())
			{
				chess_board_.SelectCoordinates(board_mouse_position);
			}
			else
			{
				chess_board_.MoveSelectedPieceToCoordinates(board_mouse_position);
			}
		}
	}
	else if (auto key = event.getIf<sf::Event::KeyPressed>())
	{
		if (key->scancode == sf::Keyboard::Scancode::F11)
		{
			window_status_ = window_status_ == sf::State::Windowed ? sf::State::Fullscreen : sf::State::Windowed;
			CreateWindow();
		}
	}
}
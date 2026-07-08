#include <string>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Constants.h"
#include "Board.h"
#include "BitmapStore.h"
#include "FileParser.h"
#include "WindowConfiguration.h"

int main()
{		
	FileParser parser_(Constants::WindowSettingsPath);
	auto window_config = parser_.GetBlueprint<WindowConfiguration>();

	auto video_mode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(video_mode, window_config.title_, window_config.state_);

	chess::Board board{};
	board.GeneratePieces();
	board.setPosition({ 0, 0 });

	// CAMERA CONFIGURATION
	auto board_size = BitmapStore::GetInstance().GetTexture(chess::Board::kTextureKey).getSize();
	auto display_size = video_mode.size;

	auto factor = static_cast<float>(display_size.y) / display_size.x;

	sf::View main_camera{};	
	main_camera.setSize(sf::Vector2f(board_size));
	main_camera.setCenter(board.getPosition() + sf::Vector2f(board_size / 2u));
	main_camera.setViewport({ {(1.f - factor) / 2.f, 0.f}, {factor, 1.f} });

	window.setView(main_camera);

	auto current_window_status = sf::State::Windowed;

	auto times_moved = 0u;

	sf::Clock clock{};
	while (window.isOpen())
	{
		// EVENTS
		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouse->button == sf::Mouse::Button::Left)
				{
					auto mouse_position = window.mapPixelToCoords(mouse->position, main_camera);
					auto board_mouse_position = chess::Board::GetCoordinates(mouse_position);

					//std::cout << "\nX: " << board_mouse_position.x << " Y:" << board_mouse_position.y;

					if (!board.IsPositionSelected())
					{
						board.SelectPosition(board_mouse_position);
					}
					else
					{
						board.TryMoveTo(board_mouse_position);
					}
				}
			}
			else if (auto key = event->getIf<sf::Event::KeyPressed>())
			{
				if (key->scancode == sf::Keyboard::Scancode::F11)
				{
					current_window_status = current_window_status == sf::State::Windowed ? sf::State::Fullscreen : sf::State::Windowed;
					window.create(video_mode, window_config.title_, current_window_status);

					window.setView(main_camera);
				}		
			}
		}
		
		auto time  = clock.restart();
		auto delta = time.asSeconds();
		
		window.clear(window_config.background_color_);
		window.draw(board);
		window.display();
	}
}
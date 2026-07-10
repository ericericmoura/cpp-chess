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
#include "BoardConfiguration.h"

int main()
{		
	FileParser window_parser(Constants::WindowSettingsPath);
	const auto window_config = window_parser.GetBlueprint<WindowConfiguration>();

	FileParser board_parser(Constants::BoardSettingsPath);
	const auto board_config = board_parser.GetBlueprint<BoardConfiguration>();

	auto video_mode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(video_mode, window_config.title_, window_config.state_);
	auto current_window_status = window_config.state_;

	chess::Board board{board_config};
	board.GeneratePieces();

	// CAMERA CONFIGURATION
		auto board_size   = BitmapStore::GetInstance().GetTexture(board_config.texture_key_).getSize();
	auto display_size = video_mode.size;

	auto camera_size = board_size;
	camera_size.x += board_config.cell_size_.x * 2;

	auto camera_ratio = static_cast<float>(camera_size.x) / static_cast<float>(camera_size.y);
	auto factor = camera_ratio * (static_cast<float>(display_size.y) / display_size.x);

	sf::View main_camera{};	
	main_camera.setSize(sf::Vector2f(camera_size));
	main_camera.setCenter(board.getPosition() + sf::Vector2f(board_size / 2u));
	main_camera.setViewport({ {(1.f - factor) / 2.f, 0.f}, {factor, 1.f} });
	window.setView(main_camera);

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
					auto mouse_position       = window.mapPixelToCoords(mouse->position, main_camera);
					auto board_mouse_position = board.GetCoordinatesFromPosition(mouse_position);

					if (!board.IsCoordinateSelected())
					{
						board.SelectCoordinate(board_mouse_position);
					}
					else
					{
						board.MoveSelectedPieceToCoordinate(board_mouse_position);
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
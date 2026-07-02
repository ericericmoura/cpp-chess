#include <string>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include "Board.h"
#include "BitmapStore.h"

constexpr int GrayColor = 57;
const std::string WindowTitle = "Chess";

int main()
{		
	auto video_mode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(video_mode, WindowTitle, sf::State::Windowed);

	chess::Board board{};
	board.GeneratePieces();

	auto delay = 2.f;
	auto time_elapsed = 0.f;

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

	bool moved = false;

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
			if (auto key = event->getIf<sf::Event::KeyPressed>())
			{
				if (key->scancode == sf::Keyboard::Scancode::F11)
				{
					current_window_status = current_window_status == sf::State::Windowed ? sf::State::Fullscreen : sf::State::Windowed;
					window.create(video_mode, WindowTitle, current_window_status);

					window.setView(main_camera);
				}
				else if (key->scancode == sf::Keyboard::Scancode::E)
				{
					board.SelectPosition({1, 2});					
				}
				else if (!moved && key->scancode == sf::Keyboard::Scancode::R)
				{
					board.TryMoveTo({ 1, 8 });
					moved = true;
				}
			}
		}
		
		auto time  = clock.restart();
		auto delta = time.asSeconds();
		
		window.clear(sf::Color(250, 142, 200));
		window.draw(board);
		window.display();
	}
}
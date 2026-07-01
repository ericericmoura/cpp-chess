
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>

#include "Piece.h"
#include <SFML/System/Clock.hpp>

int main()
{
	auto video_mode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(video_mode, "Chess", sf::State::Windowed);

	chess::Piece piece{};	
	piece.SetBoardPosition({1, 1});

	auto delay = 2.f;
	auto time_elapsed = 0.f;

	sf::Clock clock{};
	while (window.isOpen())
	{
		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		auto time  = clock.restart();
		auto delta = time.asSeconds();
		
		time_elapsed += delta;
		if (time_elapsed > delay)
		{
			piece.SetBoardPosition({ 8, 1 });
		}
		if (time_elapsed > delay * 2)
		{
			piece.SetBoardPosition({ 8, 8 });
		}

		window.clear(sf::Color::Blue);
		window.draw(piece);
		window.display();
	}
}
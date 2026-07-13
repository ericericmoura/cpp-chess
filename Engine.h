#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window/Event.hpp>

#include "WindowConfiguration.h"
#include "BoardConfiguration.h"
#include "Board.h"

namespace chess
{

class Engine
{
public:	
	Engine() noexcept;
	void Run();

	void HandleEvent(sf::Event event);
	void Update(float delta);
	void Render();

private:
	file_io::WindowConfiguration window_config_{};
	file_io::BoardConfiguration  board_config_ {};

	sf::RenderWindow window_{};
	sf::VideoMode    current_video_mode_{};
	sf::State	     window_status_{};
	sf::View         main_camera_  {};

	Board chess_board_{};

	void InitializeCameraForChessBoard(const sf::Vector2u& display_size, sf::View& camera, file_io::BoardConfiguration& board_config) noexcept;
	void CreateWindow() noexcept;
};

} // namespace chess
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>

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

private:
	WindowConfiguration window_config_{};
	BoardConfiguration  board_config_ {};

	sf::RenderWindow window_{};
	sf::VideoMode    current_video_mode_{};
	sf::State window_status_{};
	sf::View  main_camera   {};

	Board chess_board_{};
};

} // namespace chess
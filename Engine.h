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
#include "UI/WidgetFactory.h"
#include "UI/Text.h"
#include "UI/Container.h"
#include <SFML/Window/Mouse.hpp>
#include "UI/Button.h"
#include <SFML/System/Time.hpp>

namespace chess
{

class Engine
{
public:	
	inline static sf::Vector2f local_mouse_position_{};
	inline static sf::Time elapsed_time_{};

	Engine() noexcept;
	void Run();

	void HandleEvent(sf::Event event);
	void Update(float delta);
	void Render();

	sf::Vector2u GetMousePositionInUICoords()
	{
		return sf::Vector2u(window_.mapPixelToCoords(sf::Mouse::getPosition(window_), ui_camera_));
	}

private:
	file_io::WindowConfiguration window_config_{};
	file_io::BoardConfiguration  board_config_ {};

	sf::RenderWindow window_{};
	sf::VideoMode    current_video_mode_{};
	sf::State	     window_status_{};
	sf::View         main_camera_{};
	sf::View         ui_camera_  {};

	Board chess_board_{};

	ui::WidgetFactory widget_factory_{};
	ui::Text hud_text_;
	
	ui::Container promotion_container_{};

	void InitializeCameraForChessBoard(const sf::Vector2u& display_size, sf::View& camera, file_io::BoardConfiguration& board_config) noexcept;
	void CreateWindow() noexcept;
};

} // namespace chess
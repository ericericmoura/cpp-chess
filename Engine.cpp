#include "Engine.h"

#include <cassert>

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>

#include "BitmapStore.h"
#include "FileParser.h"
#include "Constants.h"
#include "BoardConfiguration.h"
#include "WindowConfiguration.h"

chess::Engine::Engine() noexcept
{
	FileParser window_parser(Constants::WindowSettingsPath);
	auto window_config_opt = window_parser.GetBlueprint<WindowConfiguration>();

	FileParser board_parser(Constants::BoardSettingsPath);
	auto board_config_opt = board_parser.GetBlueprint<BoardConfiguration>();

	assert(board_config_opt && window_config_opt);
	window_config_ = window_config_opt.value();
	board_config_  = board_config_opt .value();

	current_video_mode_ = sf::VideoMode::getDesktopMode();
	CreateWindow();
	InitializeCameraForChessBoard(current_video_mode_.size, main_camera_, board_config_);
	window_.setView(main_camera_);

	chess_board_.SetConfig(board_config_);
	chess_board_.GeneratePieces();
}

void chess::Engine::Run()
{
	sf::Clock clock{};	
	while (window_.isOpen())
	{
		// EVENTS
		while (auto event = window_.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window_.close();
			}
			else if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouse->button == sf::Mouse::Button::Left)
				{
					auto mouse_position = window_.mapPixelToCoords(mouse->position, main_camera_);
					auto board_mouse_position = chess_board_.GetCoordinatesFromPosition(mouse_position);

					if (!chess_board_.IsCoordinateSelected())
					{
						chess_board_.SelectCoordinate(board_mouse_position);
					}
					else
					{
						chess_board_.MoveSelectedPieceToCoordinate(board_mouse_position);
					}
				}
			}
			else if (auto key = event->getIf<sf::Event::KeyPressed>())
			{
				if (key->scancode == sf::Keyboard::Scancode::F11)
				{
					window_status_ = window_status_ == sf::State::Windowed ? sf::State::Fullscreen : sf::State::Windowed;
					CreateWindow();					
				}
			}
		}

		auto time  = clock.restart ();
		auto delta = time.asSeconds();

		Update();

		Render();		
	}

}

void chess::Engine::InitializeCameraForChessBoard(const sf::Vector2u& display_size, sf::View& camera, BoardConfiguration& board_config) noexcept
{
	auto board_size = BitmapStore::GetInstance().GetTexture(board_config.texture_key_).getSize();

	auto camera_size = board_size;
	camera_size.x += board_config.cell_size_.x * 2;

	auto camera_ratio = static_cast<float>(camera_size.x) / static_cast<float>(camera_size.y);
	auto factor = camera_ratio * (static_cast<float>(display_size.y) / display_size.x);

	camera.setSize(sf::Vector2f(camera_size));
	camera.setCenter(chess_board_.getPosition() + sf::Vector2f(board_size / 2u));
	camera.setViewport({ {(1.f - factor) / 2.f, 0.f}, {factor, 1.f} });

}

void chess::Engine::CreateWindow() noexcept
{
	window_.create(current_video_mode_, window_config_.title_, window_status_);
}

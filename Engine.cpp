#include "Engine.h"

#include <cassert>

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "BitmapStore.h"
#include "FileParser.h"
#include "Constants.h"
#include "BoardConfiguration.h"
#include "WindowConfiguration.h"

chess::Engine::Engine() noexcept
{
	file_io::FileParser window_parser(constants::WindowSettingsPath);
	auto window_config_opt = window_parser.GetBlueprint<file_io::WindowConfiguration>();

	file_io::FileParser board_parser(constants::BoardSettingsPath);
	auto board_config_opt = board_parser.GetBlueprint<file_io::BoardConfiguration>();

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
		while (auto event_opt = window_.pollEvent())
		{
			if (event_opt)
			{
				HandleEvent(event_opt.value());
			}
		}
		auto time  = clock.restart ();
		auto delta = time.asSeconds();

		Update(delta);
		Render();		
	}
}

void chess::Engine::Update(float delta)
{}

void chess::Engine::Render()
{
	window_.clear(window_config_.background_color_);
	window_.draw(chess_board_);
	window_.display();
}

void chess::Engine::InitializeCameraForChessBoard(const sf::Vector2u& display_size, sf::View& camera, file_io::BoardConfiguration& board_config) noexcept
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

#include "Engine.h"

#include <cassert>
#include <string>
#include <optional>
#include <iostream>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Mouse.hpp>

#include "BitmapStore.h"
#include "FileParser.h"
#include "Constants.h"
#include "BoardConfiguration.h"
#include "WindowConfiguration.h"
#include "Team.h"
#include "PieceType.h"

chess::Engine::Engine() noexcept
	: hud_text_(constants::MainFontPath)
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

	ui_camera_.setSize  (sf::Vector2f(current_video_mode_.size));	
	ui_camera_.setCenter(sf::Vector2f(current_video_mode_.size) / 2.f);

	chess_board_.SetConfig(board_config_);
	chess_board_.GeneratePieces();
	chess_board_.OnTurnChanged([this](auto team)
	{
		std::string text_string = "";
		if (team == Team::Black)
		{
			text_string = constants::BlackTurnText;
			hud_text_.GetText().setFillColor(sf::Color::Black);
			hud_text_.GetText().setOutlineColor(sf::Color::White);
		}
		else
		{
			text_string = constants::WhiteTurnText;
			hud_text_.GetText().setFillColor(sf::Color::White);
			hud_text_.GetText().setOutlineColor(sf::Color::Black);
		}
		hud_text_.GetText().setString(text_string.data());
	});

	hud_text_.SetResponsivePosition({ .5f, .04f });
	hud_text_.GetText().setString(constants::WhiteTurnText.data());
	hud_text_.GetText().setCharacterSize(60);
	hud_text_.GetText().setFillColor(sf::Color::White);
	hud_text_.GetText().setOutlineThickness(3.f);
	hud_text_.GetText().setOutlineColor(sf::Color::Black);
	hud_text_.GetText().setOrigin(hud_text_.GetText().getGlobalBounds().size / 2.f);

	chess_board_.OnPromotionWidgetRequested([this](auto team, auto pos)
		{
			auto translated_widget_position = window_.mapPixelToCoords(window_.mapCoordsToPixel(pos, main_camera_), ui_camera_);
			translated_widget_position.x += 200;
			if (team == Team::Black)
			{
				translated_widget_position.y -= 300;
			}

			auto coordinates = chess_board_.GetCoordinatesFromPosition(pos);

			pawn_promotion_widget_ = widget_factory_.CreatePromotionWidget
			(
				team, 
				translated_widget_position,
				[this, coordinates]() { chess_board_.Promote(coordinates, PieceType::Knight); pawn_promotion_widget_pending_deletion_ = true; },
				[this, coordinates]() { chess_board_.Promote(coordinates, PieceType::Bishop); pawn_promotion_widget_pending_deletion_ = true; },
				[this, coordinates]() { chess_board_.Promote(coordinates, PieceType::Rook);   pawn_promotion_widget_pending_deletion_ = true; },
				[this, coordinates]() { chess_board_.Promote(coordinates, PieceType::Queen);  pawn_promotion_widget_pending_deletion_ = true; });

			std::cout << pawn_promotion_widget_.has_value();
		});
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
		elapsed_time_ += time;
		auto delta = time.asSeconds();

		if (pawn_promotion_widget_.has_value())
		{
			pawn_promotion_widget_->HandleInput(GetMousePositionInUICoords());
		}
		Update(delta);
		Render();
	}
}

void chess::Engine::Update(float delta)
{
	local_mouse_position_ = window_.mapPixelToCoords(sf::Mouse::getPosition(window_), main_camera_);
	chess_board_.Update(delta);
	hud_text_.Update(current_video_mode_.size);

	if (pawn_promotion_widget_.has_value())
	{
		pawn_promotion_widget_->Update(current_video_mode_.size);
	}

	if (pawn_promotion_widget_pending_deletion_)
	{
		pawn_promotion_widget_pending_deletion_ = false;
		pawn_promotion_widget_ = {};
	}
}

void chess::Engine::Render()
{
	window_.clear(window_config_.background_color_);

	window_.setView(main_camera_);
	window_.draw(chess_board_);

	window_.setView(ui_camera_);
	window_.draw(hud_text_);
	if (pawn_promotion_widget_.has_value())
	{
		window_.draw(*pawn_promotion_widget_);
	}
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
	window_.setView(main_camera_);
}

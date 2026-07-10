#include "Engine.h"

#include <cassert>

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

	chess_board_.SetConfig(board_config_);
}

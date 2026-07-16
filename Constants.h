#pragma once

#include <string_view>

namespace chess
{
namespace constants
{

constexpr std::string_view WindowSettingsPath = "./config/window_settings.dat";
constexpr std::string_view BoardSettingsPath  = "./config/board_settings.dat";
constexpr std::string_view MainFontPath = "./fonts/pixel_game_by_suhadidesign.otf";

constexpr std::string_view WhiteTurnText = "White's Turn";
constexpr std::string_view BlackTurnText = "Black's Turn";

} // namespace constants
} // namespace chess

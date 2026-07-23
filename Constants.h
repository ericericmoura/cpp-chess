#pragma once

#include <string_view>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace chess
{
namespace constants
{

constexpr std::string_view WindowSettingsPath = "./config/window_settings.dat";
constexpr std::string_view BoardSettingsPath  = "./config/board_settings.dat";
constexpr std::string_view MainFontPath = "./fonts/pixel_game_by_suhadidesign.otf";

constexpr std::string_view WhiteTurnText = "White's Turn";
constexpr std::string_view BlackTurnText = "Black's Turn";

constexpr std::string_view BlackKnightTextureKey = "./graphics/black_knight.png";
constexpr std::string_view BlackRookTextureKey   = "./graphics/black_rook.png";
constexpr std::string_view BlackBishopTextureKey = "./graphics/black_bishop.png";
constexpr std::string_view BlackQueenTextureKey  = "./graphics/black_queen.png";

constexpr sf::Color BlackWidgetBackgroundColor{ 34 , 32 , 33  };
constexpr sf::Color BlackWidgetOutlineColor   { 190, 189, 184 };
constexpr float BlackWidgetOutlineThickness = 2.f;

constexpr sf::Vector2u WhiteKingStartingCoordinates = { 4, 7 };
constexpr sf::Vector2u BlackKingStartingCoordinates = { 4, 0 };

} // namespace constants
} // namespace chess

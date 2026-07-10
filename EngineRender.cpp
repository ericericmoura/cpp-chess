#include "Engine.h"

void chess::Engine::Render()
{
	window_.clear(window_config_.background_color_);
	window_.draw(chess_board_);
	window_.display();
}
#pragma once

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Blueprint.h"

struct BoardConfiguration : public Blueprint
{
	sf::Vector2u board_size_  {};
	sf::Vector2u margin_      {};
	sf::Vector2u cell_size_   {};
	std::string  texture_key_ {};

	void Parse(std::vector<std::string>& data) override;
};


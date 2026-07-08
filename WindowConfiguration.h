#pragma once

#include <string>

#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Graphics/Color.hpp>

#include "Blueprint.h"
#include <vector>

struct WindowConfiguration : public Blueprint
{
	std::string title_ = "";

	sf::State state_           {};
	sf::Color background_color_{};

	void Parse(std::vector<std::string>& data) noexcept override;

private:
	sf::State GetStateFromString(const std::string& value) const noexcept;
};
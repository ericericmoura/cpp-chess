#pragma once

#include <string>
#include <vector>

#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Graphics/Color.hpp>

#include "Blueprint.h"

namespace chess
{
namespace file_io
{

struct WindowConfiguration : public Blueprint
{
	std::string title_ = "";

	sf::State state_{};
	sf::Color background_color_{};

	void Parse(std::vector<std::string>& data) override;

private:
	sf::State GetStateFromString(const std::string& value) const noexcept;
};

} // namespace file_io
} // namespace chess
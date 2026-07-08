#include "WindowConfiguration.h"

#include <string>
#include <vector>

#include <SFML/Window/WindowEnums.hpp>

#include "GetFileTagByRegex.h"
#include "get_name.h"

void WindowConfiguration::Parse(std::vector<std::string>& data) noexcept
{
	for (const auto& line : data)
	{
		if (line.at(0) == kCommentChar)
		{
			continue;
		}
		auto [tag, value] = GetFileTagByRegex(line);
		if (tag == GET_NAME(title_))
		{
			title_ = value;
		}
		else if (tag == GET_NAME(background_color_))
		{

		}
		else if (tag == GET_NAME(state_))
		{
			state_ = GetStateFromString(value);
		}
	}
}

sf::State WindowConfiguration::GetStateFromString(const std::string& value) const noexcept
{
	if (value == "windowed")
	{
		return sf::State::Windowed;
	}
	if (value == "fullscreen")
	{
		return sf::State::Fullscreen;
	}
	return {};
}

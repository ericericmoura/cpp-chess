#include "WindowConfiguration.h"

#include <string>
#include <vector>

#include <SFML/Window/WindowEnums.hpp>

#include "GetFileTagByRegex.h"
#include "GET_NAME.h"
#include "StringToColor.h"

void chess::file_io::WindowConfiguration::Parse(std::vector<std::string>& data)
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
			background_color_ = StringToColor(value);
		}
		else if (tag == GET_NAME(state_))
		{
			state_ = GetStateFromString(value);
		}
	}
}

sf::State chess::file_io::WindowConfiguration::GetStateFromString(const std::string& value) const noexcept
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

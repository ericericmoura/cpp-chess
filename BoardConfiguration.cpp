#include "BoardConfiguration.h"

#include <string>
#include <vector>

#include "GetFileTagByRegex.h"
#include "StringToVector2.h"
#include "GET_NAME.h"

void BoardConfiguration::Parse(std::vector<std::string>& data)
{
	for (const auto& line : data)
	{
		if (line.at(0) == kCommentChar)
		{
			continue;
		}
		auto [tag, value] = GetFileTagByRegex(line);
		if (tag == GET_NAME(texture_key_))
		{
			texture_key_ = value;
		}
		else if (tag == GET_NAME(board_size_))
		{
			board_size_ = StringToVector2<unsigned int>(value);
		}
		else if (tag == GET_NAME(margin_))
		{
			margin_ = StringToVector2<unsigned int>(value);
		}
		else if (tag == GET_NAME(cell_size_))
		{
			cell_size_ = StringToVector2<unsigned int>(value);
		}
	}
}

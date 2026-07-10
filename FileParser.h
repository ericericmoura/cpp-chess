#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <string_view>
#include <optional>
#include <utility>

#include "Blueprint.h"

class FileParser
{
public:
	FileParser(std::string_view filepath) noexcept
		: input_file_(filepath.data())
	{
#ifdef _DEBUG
		if (!input_file_.good())
		{
			std::cout << "Error while reading file at: ´" << filepath << "´";
		}
#endif // _DEBUG
	}

	template<BlueprintDerived BP>	
	std::optional<BP> GetBlueprint()
	{
		auto data = GetRawData();
		if (!data.has_value())
		{
			return {};
		}
		BP blueprint{};
		blueprint.Parse(data.value());
		return blueprint;
	}

	std::optional<std::vector<std::string>> GetRawData()
	{
		if (!input_file_.is_open())
		{
			return {};
		}
		std::optional<std::vector<std::string>> result(std::in_place);
		for (std::string line; std::getline(input_file_, line);)
		{
			result->push_back(line);
		}
		return result;
	}

private:
	std::ifstream input_file_{};
};
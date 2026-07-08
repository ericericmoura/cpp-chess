#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#include "Blueprint.h"
#include <string_view>

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
	BP GetBlueprint()
	{
		std::vector<std::string> data{};
		if (!GetRawData(data))
		{
			return {};
		}
		BP blueprint{};
		blueprint.Parse(data);
		return blueprint;
	}

	bool GetRawData(std::vector<std::string>& out)
	{
		if (!input_file_.is_open())
		{
			return false;
		}
		out.clear();
		for (std::string line; std::getline(input_file_, line);)
		{
			out.push_back(line);
		}
		return true;
	}

private:
	std::ifstream input_file_{};
};
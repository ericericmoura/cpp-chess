#pragma once

#include <vector>
#include <string>
#include <concepts>

struct Blueprint
{
	static constexpr char kCommentChar = '#';

	virtual void Parse(std::vector<std::string>& data) = 0;
};

template<typename BP>
concept BlueprintDerived =
	std::derived_from<BP, Blueprint>&&
	std::default_initializable<BP>;

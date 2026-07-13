#pragma once

#include <vector>
#include <string>
#include <concepts>

namespace chess
{
namespace file_io
{

struct Blueprint
{
	static constexpr char kCommentChar = '#';

	virtual void Parse(std::vector<std::string>& data) = 0;
};

template<typename BP>
concept BlueprintDerived =
	std::derived_from<BP, Blueprint>&&
	std::default_initializable<BP>;

} // namespace file_io
} // namespace chess
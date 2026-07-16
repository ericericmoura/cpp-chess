#pragma once

#include <unordered_map>
#include <string>

#include <SFML/Graphics/Font.hpp>

namespace chess
{
namespace ui
{

class FontStore
{
public:	
	FontStore(const FontStore&) = delete;
	FontStore& operator=(const FontStore&) = delete;
	FontStore(FontStore&&) = delete;
	FontStore& operator=(FontStore&&) = delete;

	static FontStore& GetInstance() noexcept;	

	sf::Font& GetFont(const std::string& key) const noexcept;

private:
	mutable std::unordered_map<std::string, sf::Font> fonts_map_{};

	FontStore() = default;
};

} // namespace ui
} // namespace chess
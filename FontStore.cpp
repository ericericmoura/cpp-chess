#include "UI/FontStore.h"

#include <string>
#include <iostream>

#include <SFML/Graphics/Font.hpp>

chess::ui::FontStore& chess::ui::FontStore::GetInstance() noexcept
{
	static FontStore store;
	return store;
}

sf::Font& chess::ui::FontStore::GetFont(const std::string& key) const noexcept
{
	auto it = fonts_map_.find(key);
	if (it == fonts_map_.end())
	{
		auto [inserted_it, success] = fonts_map_.try_emplace(key, sf::Font());
		if (!success || !inserted_it->second.openFromFile(key))
		{
#ifdef _DEBUG
			std::cout << "Failed to find font by key `" << key << "`";
#endif // _DEBUG
		}
		return inserted_it->second;
	}
	return it->second;
}

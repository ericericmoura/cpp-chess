#include "BitmapStore.h"

#include <iostream>
#include <string>

#include <SFML/Graphics/Texture.hpp>

BitmapStore& BitmapStore::GetInstance() noexcept
{
	static BitmapStore instance;
	return instance;
}

sf::Texture& BitmapStore::GetTexture(const std::string& key) const noexcept
{
	auto it = bitmaps_.find(key);
	if (it == bitmaps_.end())
	{
		auto [inserted_it, success] = bitmaps_.try_emplace(key, sf::Texture());
		if (!success || !inserted_it->second.loadFromFile(key))
		{
#ifdef _DEBUG
			std::cout << "ERROR at BitmapStore: Failed to load texture by key `" << key << "`";
#endif // _DEBUG
		}
		return inserted_it->second;
	}
	return it->second;
}

#pragma once

#include <string>
#include <unordered_map>

#include <SFML/Graphics/Texture.hpp>

namespace chess
{

class BitmapStore
{
public:
	BitmapStore(BitmapStore&&) = delete;
	BitmapStore& operator=(BitmapStore&&) = delete;
	BitmapStore(const BitmapStore&) = delete;
	BitmapStore& operator=(const BitmapStore&) = delete;

	static BitmapStore& GetInstance() noexcept;

	sf::Texture& GetTexture(const std::string& key) const noexcept;

private:
	mutable std::unordered_map<std::string, sf::Texture> bitmaps_{};

	BitmapStore() = default;
};

} // namespace chess
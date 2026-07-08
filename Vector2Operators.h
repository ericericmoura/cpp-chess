#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf
{

template <typename T>
inline bool operator>(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
{
	return left.x > right.x && left.y > right.y;
}

template <typename T>
inline bool operator>=(const sf::Vector2<T>& left, const sf::Vector2<T>& right)
{
	return left.x >= right.x && left.y >= right.y;
}

template <typename T>
inline bool operator<(const sf::Vector2<T>& left, const sf::Vector2<T> right)
{
	return left.x < right.x && left.y < right.y;
}

template <typename T>
inline sf::Vector2<T> operator*(const sf::Vector2<T>& left, const sf::Vector2<T> right)
{
	return sf::Vector2<T>(left.x * right.x, left.y * right.y);
}

template <typename T>
inline sf::Vector2<T> operator/(const sf::Vector2<T>& left, const sf::Vector2<T> right)
{
	return sf::Vector2<T>(left.x / right.x, left.y / right.y);
}
	
}
#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "StandardGraphicsComponent.h"

namespace chess {

class Piece : public sf::Transformable, public sf::Drawable
{
public:
	Piece() = default;
	Piece(sf::Vector2i starting_pos) noexcept;

	void SetBoardPosition(sf::Vector2i pos) noexcept;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	StandardGraphicsComponent graphics_{};
};

}
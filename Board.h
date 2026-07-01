#pragma once

#include <unordered_map>
#include <string>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Vector2Hash.h"
#include "Piece.h"
#include "StandardGraphicsComponent.h"

namespace chess {

class Board : public sf::Transformable, public sf::Drawable
{
public:
	inline static constexpr sf::Vector2u kBoardSize  = { 8 , 8  };
	inline static constexpr sf::Vector2u kMargin     = { 40, 40 };
	inline static constexpr sf::Vector2u kCellSize   = { 22, 22 };
	inline static const std::string kTextureKey = "graphics/board_alt.png";

	Board();

	void GeneratePieces() noexcept;	

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::unordered_map<sf::Vector2u, Piece, Vec2uHash> board_{};

	StandardGraphicsComponent graphics_{};
};

}
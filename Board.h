#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Vector2Hash.h"
#include "Piece.h"
#include "StandardGraphicsComponent.h"
#include "PieceFactory.h"

namespace chess {

using PiecesMap = std::unordered_map<sf::Vector2u, Piece, Vec2uHash>;

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
	
	void SelectPosition(sf::Vector2u position) noexcept;
	void TryMoveTo(sf::Vector2u target) noexcept;

	void CaptureAt(sf::Vector2u position) noexcept;
	const Piece* GetPieceAt(sf::Vector2u position) const noexcept;
	bool IsPositionOccupied(const sf::Vector2u& position) const noexcept;
	
	inline bool IsPositionSelected() const noexcept
	{
		return position_selected_;
	}

	static sf::Vector2f GetPositionInPixels(sf::Vector2u board_position) noexcept;
	static sf::Vector2u GetCoordinates(sf::Vector2f position) noexcept;
	static bool IsCoordinatesValid(sf::Vector2u position) noexcept;

private:
	PieceFactory factory_{};

	PiecesMap active_pieces_{};
	std::vector<Piece> inactive_pieces_{};

	sf::Vector2u selected_position_{};

	StandardGraphicsComponent graphics_{};

	bool position_selected_ = false;
};

}
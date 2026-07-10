#pragma once

#include <unordered_map>
#include <vector>
#include <optional>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "Vector2Hash.h"
#include "Piece.h"
#include "StandardGraphicsComponent.h"
#include "BoardGraphicsComponent.h"
#include "PieceFactory.h"
#include "BoardConfiguration.h"

namespace chess {

using PiecesMap = std::unordered_map<sf::Vector2u, Piece, Vec2uHash>;

class Board : public sf::Transformable, public sf::Drawable
{
public:
	Board(BoardConfiguration board_config) noexcept;
	
	void GeneratePieces() noexcept;	
	
	void SelectCoordinate(sf::Vector2u position) noexcept;
	void MoveSelectedPieceToCoordinate(sf::Vector2u target) noexcept;

	void CaptureAtCoordinate(sf::Vector2u position) noexcept;

	const Piece* GetPieceAtCoordinate(sf::Vector2u position) const noexcept;
	bool IsCoordinateOccupied(const sf::Vector2u& position) const noexcept;

	const BoardConfiguration& GetConfig() const noexcept;
	
	inline bool IsCoordinateSelected() const noexcept
	{
		return selected_coordinate_.has_value();
	}
	
	sf::Vector2f GetPositionFromCoordinates(const sf::Vector2u& coordinates) const noexcept;
	sf::Vector2u GetCoordinatesFromPosition(const sf::Vector2f& position) const noexcept;
	bool         IsCoordinatesWithinBounds (const sf::Vector2u& position) const noexcept;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	StandardGraphicsComponent graphics_{};
	BoardGraphicsComponent    board_graphics_{};
	PieceFactory factory_{};
	BoardConfiguration board_config_{};

	PiecesMap active_pieces_{};
	std::vector<Piece> inactive_white_pieces_{};	
	std::vector<Piece> inactive_black_pieces_{};

	std::optional<sf::Vector2u> selected_coordinate_ {};
};

} // namespace chess
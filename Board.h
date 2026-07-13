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
#include "Team.h"

namespace chess {

using PiecesMap = std::unordered_map<sf::Vector2u, Piece, Vec2uHash>;

class Board : public sf::Transformable, public sf::Drawable
{
public:
	Board() = default;
	Board(file_io::BoardConfiguration board_config) noexcept;

	void GeneratePieces() noexcept;	
	
	void SelectCoordinates(sf::Vector2u coords) noexcept;
	void MoveSelectedPieceToCoordinates(sf::Vector2u target) noexcept;

	void CaptureAtCoordinates(sf::Vector2u coords) noexcept;

	const Piece* GetPieceAtCoordinates(sf::Vector2u coords) const noexcept;
	bool IsCoordinatesOccupied(const sf::Vector2u& coords) const noexcept;

	void SetConfig(file_io::BoardConfiguration config) noexcept;
	const file_io::BoardConfiguration& GetConfig() const noexcept;
	
	inline bool IsCoordinatesSelected() const noexcept
	{
		return selected_coordinate_.has_value();
	}
	
	sf::Vector2f GetPositionFromCoordinates(const sf::Vector2u& coords) const noexcept;
	sf::Vector2u GetCoordinatesFromPosition(const sf::Vector2f& position) const noexcept;
	bool         IsCoordinatesWithinBounds (const sf::Vector2u& coords) const noexcept;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	StandardGraphicsComponent graphics_{};
	BoardGraphicsComponent    board_graphics_{};
	PieceFactory factory_{};
	file_io::BoardConfiguration board_config_{};

	PiecesMap active_pieces_{};
	std::vector<Piece> inactive_white_pieces_{};	
	std::vector<Piece> inactive_black_pieces_{};	

	std::optional<sf::Vector2u> selected_coordinate_ {};

	Team team_to_play_ = Team::White;

	sf::Vector2u white_king_coords_{};
	sf::Vector2u black_king_coords_{};
};

} // namespace chess
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "StandardGraphicsComponent.h"
#include "Team.h"
#include "Vector2Hash.h"
#include "PieceType.h"

namespace chess {

class MovementComponent;

class Piece : public sf::Transformable, public sf::Drawable
{
public:
	Piece() = default;
	Piece(Team team, const std::string& texture_key, sf::Vector2u starting_pos) noexcept;
	
	Piece(Piece&&) noexcept = default;
	Piece& operator=(Piece&&) noexcept = default;

	Piece(const Piece&) = delete;
	Piece& operator=(const Piece&) = delete;

	void AddMovementComponent(std::unique_ptr<MovementComponent> comp) noexcept;
	bool TryMove(PiecesMap& pieces, sf::Vector2u position) noexcept;

	void SetBoardPosition(sf::Vector2u pos) noexcept;
	Team GetTeam() const noexcept
	{
		return team_;
	}

	PieceType GetPieceType() const noexcept
	{
		return type_;
	}

	template <typename T>
	T* GetComponentByType()
	{
		for (auto& component : movement_components_)
		{
			if (auto cast_result = dynamic_cast<T*>(component.get()))
			{
				return cast_result;
			}
		}
		return nullptr;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<std::unique_ptr<MovementComponent>> movement_components_;

	StandardGraphicsComponent graphics_{};
	Team team_{};
	PieceType type_{};
};

using PiecesMap = std::unordered_map<sf::Vector2u, Piece, Vec2uHash>;
}
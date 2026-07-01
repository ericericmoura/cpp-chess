#pragma once

#include <string>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Vector2.hpp>

#include "StandardGraphicsComponent.h"
#include "Team.h"

namespace chess {

class Piece : public sf::Transformable, public sf::Drawable
{
public:
	Piece() = default;
	Piece(Team team, const std::string& texture_key, sf::Vector2u starting_pos) noexcept;

	void SetBoardPosition(sf::Vector2u pos) noexcept;
	Team GetTeam() const noexcept
	{
		return team_;
	}

	void Capture() noexcept 
	{};

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	StandardGraphicsComponent graphics_{};
	Team team_{};
};

}
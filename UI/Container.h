#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "Element.h"

namespace chess
{
namespace ui
{

class Container : public Element
{
public:	
	Container() = default;

	Container(const Container&) = delete;
	Container& operator=(const Container&) = delete;

	Container(Container&&) = default;
	Container& operator=(Container&&) = default;

	void SetIsVertical(bool value) noexcept;
	void SetSpacing(float value) noexcept;
	void SetPadding(sf::Vector2u value) noexcept;

	void AddElement(std::unique_ptr<Element> element) noexcept;

	// Inherited via Element
	virtual void Update(sf::Vector2u window_size) override;
	virtual void HandleInput(sf::Vector2u mouse_pos) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<std::unique_ptr<Element>> elements_{};

	bool is_vertical_ = false;

	float spacing_{};

protected:
	sf::Vector2u padding_{};
};

} // namespace ui
} // namespace chess
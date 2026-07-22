#include "UI/WidgetFactory.h"

#include <memory>
#include <utility>
#include <functional>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "UI/Container.h"
#include "UI/Text.h"
#include "UI/Button.h"
#include "UI/TextureRect.h"
#include "UI/ColorRect.h"
#include "Constants.h"
#include "Team.h"
#include "PieceFactory.h"
#include "PieceType.h"

chess::ui::ColorRect chess::ui::WidgetFactory::CreatePromotionWidget(
    Team team, 
    sf::Vector2f position, 
    std::function<void()> knight_chosen_cb, 
    std::function<void()> bishop_chosen_cb, 
    std::function<void()> rook_chosen_cb, 
    std::function<void()> queen_chosen_cb)
{
    sf::Color background_color = constants::BlackWidgetBackgroundColor;
    sf::Color outline_color    = constants::BlackWidgetOutlineColor;

    if (team == Team::White)
    {
        background_color = constants::BlackWidgetOutlineColor;
        outline_color    = constants::BlackWidgetBackgroundColor;
    }

    sf::RectangleShape shape{};
    shape.setFillColor(background_color);
    shape.setOutlineThickness(constants::BlackWidgetOutlineThickness);
    shape.setOutlineColor(outline_color);

    ColorRect container{ std::move(shape) };
    container.SetPosition(position);
    container.SetIsVertical(true);
    container.SetSpacing(10);
    container.SetPadding({ 20, 25 });

    auto button_1 = std::make_unique<Button>();
    button_1->OnClicked(knight_chosen_cb);
    auto texture_1 = std::make_unique<TextureRect>(PieceFactory::GetPieceTextureKey(team, PieceType::Knight));
    texture_1->SetSize({ 75, 75 });
    button_1->AddElement(std::move(texture_1));

    auto button_2 = std::make_unique<Button>();
    button_2->OnClicked(bishop_chosen_cb);
    auto texture_2 = std::make_unique<TextureRect>(PieceFactory::GetPieceTextureKey(team, PieceType::Bishop));
    texture_2->SetSize({ 75, 75 });
    button_2->AddElement(std::move(texture_2));

    auto button_3 = std::make_unique<Button>();
    button_3->OnClicked(rook_chosen_cb);
    auto texture_3 = std::make_unique<TextureRect>(PieceFactory::GetPieceTextureKey(team, PieceType::Rook));
    texture_3->SetSize({ 75, 75 });
    button_3->AddElement(std::move(texture_3));

    auto button_4 = std::make_unique<Button>();
    button_4->OnClicked(queen_chosen_cb);
    auto texture_4 = std::make_unique<TextureRect>(PieceFactory::GetPieceTextureKey(team, PieceType::Queen));
    texture_4->SetSize({ 75, 75 });
    button_4->AddElement(std::move(texture_4));

    auto title = std::make_unique<Text>(constants::MainFontPath);
    title->GetText().setString("Promotion!");
    title->GetText().setCharacterSize(60);
    title->GetText().setFillColor(background_color);
    title->GetText().setOutlineColor(outline_color);
    title->GetText().setOutlineThickness(4.f);

    container.AddElement(std::move(title));
    container.AddElement(std::move(button_1));
    container.AddElement(std::move(button_2));
    container.AddElement(std::move(button_3));
    container.AddElement(std::move(button_4));

    return container;
}

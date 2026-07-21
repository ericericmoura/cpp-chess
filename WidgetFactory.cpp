#include "UI/WidgetFactory.h"

#include <memory>
#include <utility>
#include <iostream>

#include "UI/Container.h"
#include "UI/Text.h"
#include "UI/Button.h"
#include "UI/TextureRect.h"
#include "Constants.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include "UI/ColorRect.h"
#include <SFML/Graphics/Color.hpp>

chess::ui::Container chess::ui::WidgetFactory::CreatePromotionWidget()
{
    Container container{};
    container.SetResponsivePosition({.2f, .2f});
    container.SetIsVertical(true);
    container.SetSpacing(10);

    auto text = std::make_unique<Text>(constants::MainFontPath);
    text->GetText().setString("Item 1");

    auto text_2 = std::make_unique<Text>(constants::MainFontPath);
    text_2->GetText().setString("Item 2");

    auto text_3 = std::make_unique<Text>(constants::MainFontPath);
    text_3->GetText().setString("Item 3");

    auto text_4 = std::make_unique<Text>(constants::MainFontPath);
    text_4->GetText().setString("Item 4");

    container.AddElement(std::move(text));
    container.AddElement(std::move(text_2));
    container.AddElement(std::move(text_3));
    container.AddElement(std::move(text_4));

    return container;
}

chess::ui::ColorRect chess::ui::WidgetFactory::CreatePromotionWidgetBtn()
{
    sf::RectangleShape shape{};
    shape.setFillColor(sf::Color(190, 189, 184));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(34, 32, 33));

    ColorRect container{std::move(shape)};
    container.SetResponsivePosition({ .2f, .2f });
    container.SetIsVertical(true);
    container.SetSpacing(10);
    container.SetPadding({ 20, 25 });

    auto button_1  = std::make_unique<Button>();
    button_1->OnClicked([]() {
        std::cout << "\nKnight chosen!";
    });    
    auto texture_1 = std::make_unique<TextureRect>(constants::BlackKnightTextureKey);
    texture_1->SetSize({ 75, 75 });
    button_1->AddElement(std::move(texture_1));

    auto button_2 = std::make_unique<Button>();
    button_2->OnClicked([]() {
        std::cout << "\nBishop chosen!";
        });
    auto texture_2 = std::make_unique<TextureRect>(constants::BlackBishopTextureKey);
    texture_2->SetSize({ 75, 75 });
    button_2->AddElement(std::move(texture_2));

    auto button_3 = std::make_unique<Button>();
    button_3->OnClicked([]() {
        std::cout << "\nRook chosen!";
        });
    auto texture_3 = std::make_unique<TextureRect>(constants::BlackRookTextureKey);
    texture_3->SetSize({ 75, 75 });
    button_3->AddElement(std::move(texture_3));

    auto button_4 = std::make_unique<Button>();
    button_4->OnClicked([]() {
        std::cout << "\nQueen chosen!";
        });
    auto texture_4 = std::make_unique<TextureRect>(constants::BlackQueenTextureKey);
    texture_4->SetSize({ 75, 75 });
    button_4->AddElement(std::move(texture_4));

    auto close_text = std::make_unique<Text>(constants::MainFontPath);
    close_text->GetText().setString("close");
    close_text->GetText().setCharacterSize(40);
    close_text->GetText().setFillColor(sf::Color::Black);
    close_text->GetText().setOutlineColor(sf::Color::White);
    close_text->GetText().setOutlineThickness(4.f);

    container.AddElement(std::move(button_1));
    container.AddElement(std::move(button_2));
    container.AddElement(std::move(button_3));
    container.AddElement(std::move(button_4));
    container.AddElement(std::move(close_text));

    return container;
}

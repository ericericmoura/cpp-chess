#include "UI/WidgetFactory.h"

#include <memory>
#include <utility>

#include "UI/Container.h"
#include "UI/Text.h"
#include "Constants.h"
#include "UI/Button.h"

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

chess::ui::Button chess::ui::WidgetFactory::CreatePromotionWidgetBtn()
{
    Button container{};
    container.SetResponsivePosition({ .2f, .2f });
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

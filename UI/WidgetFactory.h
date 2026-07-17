#pragma once

#include "Container.h"
#include "Button.h"

namespace chess
{
namespace ui
{

class WidgetFactory
{
public:
	Container CreatePromotionWidget();
	Button    CreatePromotionWidgetBtn();
};

} // namespace ui
} // namespace chess
#pragma once

#include "Container.h"
#include "ColorRect.h"

namespace chess
{
namespace ui
{

class WidgetFactory
{
public:
	Container CreatePromotionWidget();
	ColorRect CreatePromotionWidgetBtn();
};

} // namespace ui
} // namespace chess
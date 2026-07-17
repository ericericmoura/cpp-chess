#pragma once

#include "Container.h"

namespace chess
{
namespace ui
{

class WidgetFactory
{
public:
	Container CreatePromotionWidget();
};

} // namespace ui
} // namespace chess
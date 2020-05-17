#pragma once

#include "GUIComponent.h"


namespace Coil
{
	class GUISameLine : public GUIComponent
	{
	public:
		GUISameLine();

		void Draw() const override;
	};
}

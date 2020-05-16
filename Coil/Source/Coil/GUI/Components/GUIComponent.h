#pragma once

namespace Coil
{
	class GUIComponent
	{
	public:
		virtual ~GUIComponent() = default;

		virtual void Draw() const = 0;
	};
}

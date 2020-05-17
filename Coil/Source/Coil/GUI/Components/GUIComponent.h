#pragma once

namespace Coil
{
	class GUIComponent
	{
	public:
		GUIComponent(const RString<> label)
			: Label(label) {}

		virtual ~GUIComponent() = default;

		virtual void Draw() const = 0;

	protected:
		RString<String> Label;
	};
}

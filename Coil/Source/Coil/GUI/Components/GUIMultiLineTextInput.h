#pragma once

#include "GUIComponent.h"


namespace Coil
{
	class GUIMultiLineTextInput : public GUIComponent
	{
	public:
		GUIMultiLineTextInput(const GUIComponentProps& properties, const RString<BString>& inputBuffer);

		void Draw() const override;


		void BindInputBuffer(const RString<BString>& inputBuffer) { InputBuffer = inputBuffer; }

		[[nodiscard]] const RString<BString>& RetrieveInputBuffer() const { return InputBuffer; }

	private:
		RString<BString> InputBuffer;
	};
}

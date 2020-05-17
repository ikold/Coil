#pragma once

#include "GUIComponent.h"


namespace Coil
{
	class GUITextInput : public GUIComponent
	{
	public:
		GUITextInput(const RString<String>& label, const RString<BString>& inputBuffer);

		void Draw() const override;


		void BindInputBuffer(const RString<BString>& inputBuffer) { InputBuffer = inputBuffer; }

		[[nodiscard]] const RString<BString>& RetrieveInputBuffer() const { return InputBuffer; }

	private:
		RString<BString> InputBuffer;
	};
}
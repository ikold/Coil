#pragma once

#include "GUIComponent.h"


namespace Coil
{
	class GUIText : public GUIComponent
	{
	public:
		GUIText(const RString<String>& textBuffer);

		void Draw() const override;

		void BindTextBuffer(const RString<String>& textBuffer) { TextBuffer = textBuffer; }

		[[nodiscard]] const RString<String>& RetrieveTextBuffer() const { return TextBuffer; }

	private:
		RString<String> TextBuffer;
	};
}

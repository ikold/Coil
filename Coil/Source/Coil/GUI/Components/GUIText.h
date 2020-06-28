#pragma once

#include "GUIComponent.h"


namespace Coil
{
	class GUIText final : public GUIComponent
	{
	public:
		GUIText(const GUIComponentProps& properties, RString<> textBuffer);

		void Draw() const override;

		void BindTextBuffer(const RString<>& textBuffer) { TextBuffer = textBuffer; }

		[[nodiscard]] const RString<>& RetrieveTextBuffer() const { return TextBuffer; }

	private:
		RString<> TextBuffer;
	};
}

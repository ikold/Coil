#pragma once

#include "GUIComponent.h"


namespace Coil
{
	/**
	 * @brief GUI Multi line text input
	 *
	 * @note Text is stored in bound RString<BString>
	 * @see GUITextInput
	 */
	class GUIMultiLineTextInput final : public GUIComponent
	{
	public:
		GUIMultiLineTextInput(const GUIComponentProps& properties, RString<BString> inputBuffer);

		void Draw() const override;

		void BindInputBuffer(const RString<BString>& inputBuffer) { InputBuffer = inputBuffer; }
		[[nodiscard]] const RString<BString>& RetrieveInputBuffer() const { return InputBuffer; }

	private:
		RString<BString> InputBuffer;
	};
}

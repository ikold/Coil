#pragma once

#include "GUIComponent.h"

#include <glm/vec2.hpp>


namespace Coil
{
	class GUIMultiLineTextInput : public GUIComponent
	{
	public:
		GUIMultiLineTextInput(const RString<String>& label, const RString<BString>& inputBuffer, const Ref<glm::vec2>& dimensions);

		void Draw() const override;


		void BindInputBuffer(const RString<BString>& inputBuffer) { InputBuffer = inputBuffer; }

		[[nodiscard]] const RString<BString>& RetrieveInputBuffer() const { return InputBuffer; }

	private:
		RString<BString> InputBuffer;
		Ref<glm::vec2> Dimensions;
	};
}

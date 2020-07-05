#include "pch.h"
#include "GUIMultiLineTextInput.h"

#include "imgui.h"


namespace Coil
{
	GUIMultiLineTextInput::GUIMultiLineTextInput(const GUIComponentProps& properties, RString<BString> inputBuffer)
		: GUIComponent(properties),
		  InputBuffer(Move(inputBuffer)) {}

	void GUIMultiLineTextInput::Draw() const
	{
		CL_PROFILE_FUNCTION()

		ImGui::InputTextMultiline(Properties.Label->CString(), InputBuffer->GetBuffer(), static_cast<size_t>(InputBuffer->GetSize()) + 1, { Properties.Width, Properties.Height }, ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) -> int
		{
			CL_PROFILE_FUNCTION()

			auto string = *static_cast<RString<BString>*>(data->UserData);

			const int32 length = data->BufTextLen;
			const int32 size   = string->GetSize();

			if (length + 8 > size)
				string->Expand(length + 8 - size);
			else if (length + 8 < size / 2)
				string->SetSize(length + 8);

			string->SetLength(length);
			data->Buf = string->GetBuffer();

			return 0;
		}, const_cast<void*>(reinterpret_cast<const void*>(&InputBuffer)));
	}
}

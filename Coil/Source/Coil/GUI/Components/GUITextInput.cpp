#include "pch.h"
#include "GUITextInput.h"

#include "imgui.h"


namespace Coil
{
	GUITextInput::GUITextInput(const GUIComponentProps& properties, RString<BString> inputBuffer)
		: GUIComponent(properties),
		  InputBuffer(Move(inputBuffer)) {}

	void GUITextInput::Draw() const
	{
		CL_PROFILE_FUNCTION()

		ImGui::InputText(Properties.Label->CString(), InputBuffer->GetBuffer(), static_cast<int32>(InputBuffer->GetSize()) + 1, ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) -> int
		{
			CL_PROFILE_FUNCTION()

			auto string = *static_cast<RString<BString>*>(data->UserData);

			const int32 length = data->BufTextLen;
			const int32 size   = data->BufSize++;

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

#include "pch.h"
#include "GUITextInput.h"

#include "imgui.h"


namespace Coil
{
	GUITextInput::GUITextInput(const RString<String>& label, const RString<BString>& inputBuffer)
		: Label(label),
		  InputBuffer(inputBuffer) {}

	void GUITextInput::Draw() const
	{
		ImGui::InputText(Label->CString(), InputBuffer->GetBuffer(), InputBuffer->GetSize(), ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) -> int
		{
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
		}, (void*)&InputBuffer);
	}
}

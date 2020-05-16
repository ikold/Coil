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
		ImGui::InputText(Label->CString(), InputBuffer->CString(), InputBuffer->GetSize(), ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) -> int
		{
			auto string = *static_cast<RString<BString>*>(data->UserData);

			const int32 length = data->BufTextLen;
			const int32 size   = string->GetSize();

			string->SetLength(length);

			if (length + 8 > size)
				string->Expand(8);
			else if (length + 8 < size / 2)
				string->SetSize(size / 2);

			return 0;
		}, (void*)&InputBuffer);
	}
}

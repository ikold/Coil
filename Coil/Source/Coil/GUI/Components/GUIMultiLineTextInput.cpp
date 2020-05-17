#include "pch.h"
#include "GUIMultiLineTextInput.h"

#include "imgui.h"


namespace Coil
{
	GUIMultiLineTextInput::GUIMultiLineTextInput(const RString<String>& label, const RString<BString>& inputBuffer, const Ref<glm::vec2>& dimensions)
		: GUIComponent(label),
		  InputBuffer(inputBuffer),
		  Dimensions(dimensions) {}

	void GUIMultiLineTextInput::Draw() const
	{
		ImGui::InputTextMultiline(Label->CString(), InputBuffer->GetBuffer(), InputBuffer->GetSize() + 1, { Dimensions->x, Dimensions->y }, ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data) -> int
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

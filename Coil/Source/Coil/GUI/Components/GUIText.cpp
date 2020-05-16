#include "pch.h"
#include "GUIText.h"

#include "imgui.h"


namespace Coil
{
	GUIText::GUIText(const RString<String>& textBuffer)
		: TextBuffer(textBuffer) {}

	void GUIText::Draw() const
	{
		ImGui::TextUnformatted(TextBuffer->CString());
	}
}

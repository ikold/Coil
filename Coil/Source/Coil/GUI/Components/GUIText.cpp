#include "pch.h"
#include "GUIText.h"

#include "imgui.h"


namespace Coil
{
	GUIText::GUIText(const GUIComponentProps& properties, const RString<String>& textBuffer)
		: GUIComponent(properties),
		  TextBuffer(textBuffer) {}

	void GUIText::Draw() const
	{
		ImGui::TextUnformatted(TextBuffer->CString());
	}
}

#include "pch.h"
#include "GUIText.h"

#include "imgui.h"


namespace Coil
{
	GUIText::GUIText(const GUIComponentProps& properties, RString<> textBuffer)
		: GUIComponent(properties),
		  TextBuffer(Move(textBuffer)) {}

	void GUIText::Draw() const
	{
		ImGui::PushItemWidth(Properties.Width);
		
		ImGui::TextUnformatted(TextBuffer->CString());

		ImGui::PopItemWidth();
	}
}

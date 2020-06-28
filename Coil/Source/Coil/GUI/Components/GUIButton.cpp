#include "pch.h"
#include "GUIButton.h"

#include "imgui.h"


namespace Coil
{
	GUIButton::GUIButton(const GUIComponentProps& properties, std::function<void()> callback)
		: GUIComponent(properties),
		  Callback(Move(callback)) {}

	void GUIButton::Draw() const
	{
		if (ImGui::Button(Properties.Label->CString(), ImVec2(Properties.Width, Properties.Height)))
			Callback();
	}
}

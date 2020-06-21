#include "pch.h"
#include "GUIButton.h"

#include "imgui.h"


namespace Coil
{
	GUIButton::GUIButton(const GUIComponentProps& properties, const std::function<void()>& callback)
		: GUIComponent(properties),
		  Callback(callback) {}

	void GUIButton::Draw() const
	{
		ImGui::PushID(Properties.ID);

		if (ImGui::Button(Properties.Label->CString()))
			Callback();

		ImGui::PopID();
	}
}

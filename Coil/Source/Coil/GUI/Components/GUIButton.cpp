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
		if (ImGui::Button(Properties.Label->CString()))
			Callback();
	}
}

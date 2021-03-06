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
		CL_PROFILE_FUNCTION_MEDIUM()

		// Draw button and if pressed call callback function
		if (ImGui::Button(Properties.Label->CString(), ImVec2(Properties.Width, Properties.Height)))
			Callback();
	}
}

#include "pch.h"
#include "GUIComponentWindow.h"

#include "imgui.h"


namespace Coil
{
	GUIComponentWindow::GUIComponentWindow(const GUIWindowProps& properties)
		: GUIWindow(properties) {}

	void GUIComponentWindow::Draw() const
	{
		CL_PROFILE_FUNCTION_MEDIUM()
		
		for (const auto& component : Components)
		{
			ImGui::PushID(component->GetProperties().ID);

			component->SetUp();
			component->Draw();
			component->CleanUp();

			ImGui::PopID();
		}
	}
}

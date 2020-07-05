#include "pch.h"
#include "GUIComponent.h"

#include "imgui.h"


namespace Coil
{
	void GUIComponent::SetUp() const
	{
		CL_PROFILE_FUNCTION()

		if (Properties.SameLine)
			ImGui::SameLine();

		ImGui::PushItemWidth(Properties.Width);
	}

	void GUIComponent::CleanUp() const
	{
		CL_PROFILE_FUNCTION()

		ImGui::PopItemWidth();
	}
}

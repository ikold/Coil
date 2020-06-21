#include "pch.h"
#include "GUIComponent.h"

#include "imgui.h"


namespace Coil
{
	void GUIComponent::SetUp() const
	{
		if (Properties.SameLine)
			ImGui::SameLine();
	}
}

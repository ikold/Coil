#include "pch.h"
#include "GUIComponent.h"

#include "imgui.h"


namespace Coil
{
	int32 GUIComponentProps::LabelID = 0;

	void GUIComponent::SetUp() const
	{
		if (Properties.SameLine)
			ImGui::SameLine();
	}
}

#include "pch.h"
#include "GUISameLine.h"

#include "imgui.h"


namespace Coil
{
	GUISameLine::GUISameLine()
		: GUIComponent("") {}

	void GUISameLine::Draw() const
	{
		ImGui::SameLine();
	}
}

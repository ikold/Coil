#include "pch.h"
#include "GUIOverlay.h"

#include "imgui.h"


namespace Coil
{
	GUIOverlay::GUIOverlay(const GUIWindowProps& properties)
		: GUIWindow(properties)
	{
		Flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDocking;
	}

	void GUIOverlay::SetUp() const
	{
		ImGui::SetNextWindowBgAlpha(0.f); // Transparent background
	}

	void GUIOverlay::Draw() const
	{
		CL_PROFILE_FUNCTION_MEDIUM()
		
		ImGui::TextUnformatted(StringReference->CString());
	}
}

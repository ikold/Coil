#include "pch.h"
#include "ImGuiOverlay.h"

namespace Coil
{
	ImGuiOverlay::ImGuiOverlay(const char* name, int width, int height)
		: ImGuiWindow(name, width, height)
	{
		Flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	}

	ImGuiOverlay::~ImGuiOverlay()
	{
	}

	void ImGuiOverlay::Draw() const
	{
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		
		ImGui::Text(TextBuffer->c_str());
	}
}
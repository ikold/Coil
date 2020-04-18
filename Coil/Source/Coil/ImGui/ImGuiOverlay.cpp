#include "pch.h"
#include "ImGuiOverlay.h"

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

namespace Coil
{
	ImGuiOverlay::ImGuiOverlay(RString name, int32 width, int32 height)
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
		
		ImGui::TextUnformatted(StringReference->CString());
	}
}
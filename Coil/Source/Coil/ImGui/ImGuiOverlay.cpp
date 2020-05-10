#include "pch.h"
#include "ImGuiOverlay.h"

#include <utility>

#include "imgui.h"


namespace Coil
{
	ImGuiOverlay::ImGuiOverlay(RString<String> name, int32 width, int32 height)
		: ImGuiWindow(std::move(name), width, height)
	{
		Flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	}

	void ImGuiOverlay::Draw() const {
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

		ImGui::TextUnformatted(StringReference->CString());
	}
}

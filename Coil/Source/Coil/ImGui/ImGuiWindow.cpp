#include "pch.h"
#include "ImGuiWindow.h"

namespace Coil
{
	ImGuiWindow::ImGuiWindow(const char* name, int width, int height)
		: Name(name), Width(width), Height(height)
	{
	}

	ImGuiWindow::~ImGuiWindow()
	{
	}

	void ImGuiWindow::OnUpdate() const
	{
		ImGui::SetNextWindowSize(ImVec2(Width, Height), ImGuiCond_FirstUseEver);
		ImGui::Begin(Name);
		Draw();
		ImGui::End();
	}
}
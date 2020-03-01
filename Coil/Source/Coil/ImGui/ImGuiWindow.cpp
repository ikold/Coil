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

	int ImGuiWindow::TextTopCulling() const
	{
		int skipedLines = (int)(ImGui::GetScrollY() / fontHeight);

		float topDummy = fontHeight * skipedLines - dummyDefaultOffset;

		if (topDummy > 0.f)
			ImGui::Dummy(ImVec2(0.0f, topDummy));

		return skipedLines;
	}

	int ImGuiWindow::TextBottomCulling(int linesTotal) const
	{
		float windowBottom = ImGui::GetScrollY() + ImGui::GetWindowSize().y;

		float bottomDummy = fontHeight * (int)(linesTotal + 1 - (windowBottom / fontHeight));

		if (bottomDummy > 0.f)
			ImGui::Dummy(ImVec2(0.0f, bottomDummy - dummyDefaultOffset));
		return 0;
	}

	int ImGuiWindow::WindowSizeInLines() const
	{
		return (int)(ImGui::GetWindowSize().y  / fontHeight);
	}
}
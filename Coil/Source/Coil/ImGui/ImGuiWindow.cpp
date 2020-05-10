#include "pch.h"
#include "ImGuiWindow.h"

#include <utility>

#include "imgui.h"

namespace Coil
{
	ImGuiWindow::ImGuiWindow(RString<String> name, int32 width, int32 height)
		: Name(std::move(name)), Width(width), Height(height)
	{
		OpenFlag = nullptr;
		Flags = 0;
	}

	void ImGuiWindow::OnImGuiRender() const
	{
		ImGui::SetNextWindowSize(ImVec2(static_cast<float32>(Width), static_cast<float32>(Height)), ImGuiCond_FirstUseEver);
		ImGui::Begin(Name->CString(), OpenFlag, Flags);
		Draw();
		ImGui::End();
	}

	int32 ImGuiWindow::TextTopCulling() const
	{
		const auto skippedLines = static_cast<int32>(ImGui::GetScrollY() / FontHeight);

		const float32 topDummy = FontHeight * skippedLines - DummyDefaultOffset;

		if (topDummy > 0.f)
			ImGui::Dummy(ImVec2(0.0f, topDummy));

		return skippedLines;
	}

	void ImGuiWindow::TextBottomCulling(int32 linesTotal) const
	{
		const float32 windowBottom = ImGui::GetScrollY() + ImGui::GetWindowSize().y;

		const float32 bottomDummy = FontHeight * (linesTotal + 1.f - windowBottom / FontHeight);

		if (bottomDummy > 0.f)
			ImGui::Dummy(ImVec2(0.0f, bottomDummy - DummyDefaultOffset));
	}

	int32 ImGuiWindow::WindowSizeInLines() const
	{
		return static_cast<int32>(ImGui::GetWindowSize().y / FontHeight);
	}
}
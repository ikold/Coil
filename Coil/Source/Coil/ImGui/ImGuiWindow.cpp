#include "pch.h"
#include "ImGuiWindow.h"

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

namespace Coil
{
	ImGuiWindow::ImGuiWindow(RString<String> name, int32 width, int32 height)
		: Name(name), Width(width), Height(height)
	{
		OpenFlag = nullptr;
		Flags = 0;
	}

	ImGuiWindow::~ImGuiWindow()
	{}

	void ImGuiWindow::OnImGuiRender() const
	{
		ImGui::SetNextWindowSize(ImVec2(static_cast<float32>(Width), static_cast<float32>(Height)), ImGuiCond_FirstUseEver);
		ImGui::Begin(Name->CString(), OpenFlag, Flags);
		Draw();
		ImGui::End();
	}

	int32 ImGuiWindow::TextTopCulling() const
	{
		int32 skipedLines = static_cast<int32>(ImGui::GetScrollY() / fontHeight);

		float32 topDummy = fontHeight * skipedLines - dummyDefaultOffset;

		if (topDummy > 0.f)
			ImGui::Dummy(ImVec2(0.0f, topDummy));

		return skipedLines;
	}

	int32 ImGuiWindow::TextBottomCulling(int32 linesTotal) const
	{
		float32 windowBottom = ImGui::GetScrollY() + ImGui::GetWindowSize().y;

		float32 bottomDummy = fontHeight * static_cast<int32>(linesTotal + 1 - (windowBottom / fontHeight));

		if (bottomDummy > 0.f)
			ImGui::Dummy(ImVec2(0.0f, bottomDummy - dummyDefaultOffset));
		return 0;
	}

	int32 ImGuiWindow::WindowSizeInLines() const
	{
		return static_cast<int32>(ImGui::GetWindowSize().y / fontHeight);
	}
}
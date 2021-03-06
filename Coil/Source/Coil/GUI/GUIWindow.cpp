#include "pch.h"
#include "GUIWindow.h"

#include "imgui.h"


namespace Coil
{
	GUIWindow::GUIWindow(GUIWindowProps properties)
		: Properties(Move(properties)),
		  OpenFlag(nullptr),
		  Flags(0) {}

	void GUIWindow::OnImGuiRender() const
	{
		CL_PROFILE_FUNCTION_MEDIUM()
		
		if (!Properties.Visible)
			return;


		ImGui::SetNextWindowSize(ImVec2(static_cast<float32>(Properties.Width), static_cast<float32>(Properties.Height)), ImGuiCond_FirstUseEver);

		SetUp();

		ImGui::Begin(Properties.Name->CString(), OpenFlag, Flags);
		Draw();
		ImGui::End();
	}

	int32 GUIWindow::TextTopCulling() const
	{
		CL_PROFILE_FUNCTION_MEDIUM()
		
		const auto skippedLines = static_cast<int32>(ImGui::GetScrollY() / FontHeight);

		const float32 topDummy = FontHeight * skippedLines - DummyDefaultOffset;

		if (topDummy > 0.f)
			ImGui::Dummy(ImVec2(0.0f, topDummy));

		return skippedLines;
	}

	void GUIWindow::TextBottomCulling(int32 linesTotal) const
	{
		CL_PROFILE_FUNCTION_MEDIUM()
		
		const float32 windowBottom = ImGui::GetScrollY() + ImGui::GetWindowSize().y;

		const float32 bottomDummy = FontHeight * (linesTotal + 1.f - windowBottom / FontHeight);

		if (bottomDummy > 0.f)
			ImGui::Dummy(ImVec2(0.0f, bottomDummy - DummyDefaultOffset));
	}

	int32 GUIWindow::WindowSizeInLines() const { return static_cast<int32>(ImGui::GetWindowSize().y / FontHeight); }
}

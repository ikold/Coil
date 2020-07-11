#include "pch.h"
#include "GUILogWindow.h"

#include "imgui.h"


namespace Coil
{
	GUILogWindow::GUILogWindow(const GUIWindowProps& properties)
		: GUIWindow(properties),
		  Buffer(nullptr) {}

	void GUILogWindow::Draw() const
	{
		CL_PROFILE_FUNCTION_HIGH()
		
		int32 skippedLines      = TextTopCulling();
		const int32 linesToDraw = WindowSizeInLines();

		if (Buffer->size() < skippedLines)
			skippedLines = Buffer->size();

		auto it  = Buffer->begin() + skippedLines;
		auto end = Buffer->end();

		if (end - it > linesToDraw)
			end = it + linesToDraw;

		for (; it != end; ++it)
		{
			ImVec4 color;
			switch ((*it)->GetLevel())
			{
			case LogLevel::fatal: color = ImVec4(0.95f, 0.1f, 0.1f, 1.f);
				break;
			case LogLevel::error: color = ImVec4(0.9f, 0.1f, 0.1f, 1.f);
				break;
			case LogLevel::warning: color = ImVec4(0.8f, 0.4f, 0.3f, 1.f);
				break;
			case LogLevel::info: color = ImVec4(0.3f, 0.3f, 1.f, 1.f);
				break;
			case LogLevel::debug: color = ImVec4(0.3f, 1.f, 0.3f, 1.f);
				break;
			case LogLevel::trace: color = ImVec4(.9f, .9f, .9f, 1.f);
				break;
			default: color = ImVec4(1.f, 1.f, 1.f, 1.f);
				break;
			}

			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted((*it)->GetHeader()->CString());
			ImGui::SameLine();
			ImGui::TextUnformatted((*it)->GetMessage()->CString());
			ImGui::PopStyleColor();
		}


		for (int32 i = 0; i < WindowSizeInLines(); ++i)
			ImGui::TextUnformatted("");

		TextBottomCulling(Buffer->size());
	}
}

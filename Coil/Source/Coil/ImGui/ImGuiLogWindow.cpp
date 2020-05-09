#include "pch.h"
#include "ImGuiLogWindow.h"

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h"

namespace Coil
{
	ImGuiLogWindow::~ImGuiLogWindow()
	{}

	void ImGuiLogWindow::Draw() const
	{
		int32 skipedLines = TextTopCulling();
		int32 linesToDraw = WindowSizeInLines();

		if (Buffer->size() < skipedLines)
			skipedLines = Buffer->size();

		auto it = Buffer->begin() + skipedLines;
		auto end = Buffer->end();

		if (end - it > linesToDraw)
			end = it + linesToDraw;

		for (; it != end; ++it)
		{
			ImVec4 color;
			switch ((*it)->GetLevel())
			{
			case LogLevel::fatal:
				color = ImVec4(0.9f, 0.1f, 0.1f, 1.f);
				break;
			case LogLevel::error:
				color = ImVec4(0.9f, 0.1f, 0.1f, 1.f);
				break;
			case LogLevel::warning:
				color = ImVec4(0.8f, 0.4f, 0.3f, 1.f);
				break;
			case LogLevel::info:
				color = ImVec4(0.3f, 0.3f, 1.f, 1.f);
				break;
			case LogLevel::debug:
				color = ImVec4(0.3f, 1.f, 0.3f, 1.f);
				break;
			case LogLevel::trace:
				color = ImVec4(1.f, 1.f, 1.f, 1.f);
				break;
			default:
				color = ImVec4(1.f, 1.f, 1.f, 1.f);
				break;
			}

			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted((*it)->GetHeader()->CString());
			ImGui::SameLine();
			ImGui::TextUnformatted((*it)->GetMessage()->CString());
			ImGui::PopStyleColor();
		}

		int tmp = WindowSizeInLines();

		for (int i = 0; i < WindowSizeInLines(); ++i)
			ImGui::TextUnformatted("");

		TextBottomCulling(Buffer->size());
	}
}
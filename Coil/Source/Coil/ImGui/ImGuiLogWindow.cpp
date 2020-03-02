#include "pch.h"
#include "ImGuiLogWindow.h"

namespace Coil
{
	ImGuiLogWindow::~ImGuiLogWindow()
	{
	}

	void ImGuiLogWindow::Draw() const
	{
		int skipedLines = TextTopCulling();
		int linesToDraw = WindowSizeInLines();

		auto it = Buffer->begin() + skipedLines;
		auto end = Buffer->end();

		if (end - it > linesToDraw)
			end = it + linesToDraw;

		for (; it != end; ++it)
		{
			ImVec4 color;
			switch ((*it)->GetLevel())
			{
			case fatal:
				color = ImVec4(0.9f, 0.1f, 0.1f, 1.f);
				break;
			case error:
				color = ImVec4(0.9f, 0.1f, 0.1f, 1.f);
				break;
			case warning:
				color = ImVec4(0.8f, 0.4f, 0.3f, 1.f);
				break;
			case info:
				color = ImVec4(0.3f, 0.3f, 1.f, 1.f);
				break;
			case debug:
				color = ImVec4(0.3f, 1.f, 0.3f, 1.f);
				break;
			case trace:
				color = ImVec4(1.f, 1.f, 1.f, 1.f);
				break;
			default:
				color = ImVec4(1.f, 1.f, 1.f, 1.f);
				break;
			}
			ImGui::TextColored(color, (*it)->GetFormatedMessage()->CString());
		}

		TextBottomCulling(Buffer->size());
	}
}
#include "pch.h"
#include "ImGuiLogWindow.h"

namespace Coil
{
	ImGuiLogWindow::~ImGuiLogWindow()
	{
	}

	void ImGuiLogWindow::Draw() const
	{
		for (auto log : *Buffer)
		{
			ImVec4 color;
			switch (log->GetLevel())
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
			ImGui::TextColored(color, log->GetFormatedMessage());
		}
	}
}
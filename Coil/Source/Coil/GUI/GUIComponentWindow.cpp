#include "pch.h"
#include "GUIComponentWindow.h"

#include "imgui.h"


namespace Coil
{
	GUIComponentWindow::GUIComponentWindow(const GUIWindowProps& properties)
		: GUIWindow(properties) {}

	void GUIComponentWindow::Draw() const
	{
		for (const auto& component : Components)
			component->Draw();
	}
}

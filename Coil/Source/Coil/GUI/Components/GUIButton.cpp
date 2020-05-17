#include "pch.h"
#include "GUIButton.h"

#include "imgui.h"


namespace Coil
{
	GUIButton::GUIButton(const RString<> label, void (*callback)())
		: GUIComponent(label),
		  Callback(callback) {}

	void GUIButton::Draw() const
	{
		if (ImGui::Button(Label->CString()))
			Callback();
	}
}

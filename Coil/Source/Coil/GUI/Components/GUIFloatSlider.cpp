#include "pch.h"
#include "GUIFloatSlider.h"

#include "imgui.h"


namespace Coil
{
	GUIFloatSlider::GUIFloatSlider(const GUIComponentProps& properties, Ref<float32> floatRef, float32 minValue, float32 maxValue)
		: GUIComponent(properties),
		  FloatRef(Move(floatRef)),
		  MinValue(minValue),
		  MaxValue(maxValue) {}

	void GUIFloatSlider::Draw() const
	{
		CL_PROFILE_FUNCTION_MEDIUM()

		ImGui::SliderFloat(Properties.Label->CString(), &*FloatRef, MinValue, MaxValue);
	}
}

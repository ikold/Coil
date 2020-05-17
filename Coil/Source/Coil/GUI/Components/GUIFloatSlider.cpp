#include "pch.h"
#include "GUIFloatSlider.h"

#include "imgui.h"


namespace Coil
{
	GUIFloatSlider::GUIFloatSlider(const GUIComponentProps& properties, const Ref<float32>& floatRef, float32 minValue, float32 maxValue)
		: GUIComponent(properties),
		  FloatRef(floatRef),
		  MinValue(minValue),
		  MaxValue(maxValue) {}

	void GUIFloatSlider::Draw() const
	{
		ImGui::SliderFloat(Properties.Label->CString(), &*FloatRef, MinValue, MaxValue);
	}
}

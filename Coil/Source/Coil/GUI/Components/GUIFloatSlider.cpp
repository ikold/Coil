#include "pch.h"
#include "GUIFloatSlider.h"

#include "imgui.h"


namespace Coil
{
	GUIFloatSlider::GUIFloatSlider(const RString<>& label, const Ref<float32>& floatRef, float32 minValue, float32 maxValue)
		: GUIComponent(label),
		  FloatRef(floatRef),
		  MinValue(minValue),
		  MaxValue(maxValue) {}

	void GUIFloatSlider::Draw() const
	{
		ImGui::SliderFloat(Label->CString(), &*FloatRef, MinValue, MaxValue);
	}
}

#include "pch.h"
#include "GUIPlotLine.h"

#include "imgui.h"


namespace Coil
{
	GUIPlotLine::GUIPlotLine(const GUIComponentProps& properties, Ref<std::vector<float32>> dataBuffer, float32 scaleMin, float32 scaleMax)
		: GUIComponent(properties),
		  DataBuffer(Move(dataBuffer)),
		  ScaleMin(scaleMin),
		  ScaleMax(scaleMax) {}

	void GUIPlotLine::Draw() const
	{
		CL_PROFILE_FUNCTION_MEDIUM()

		ImGui::PlotLines(Properties.Label->CString(), DataBuffer->_Unchecked_begin(), static_cast<int32>(DataBuffer->size()), 0, nullptr, ScaleMin, ScaleMax, { Properties.Width, Properties.Height });
	}
}

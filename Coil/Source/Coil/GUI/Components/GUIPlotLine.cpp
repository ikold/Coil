#include "pch.h"
#include "GUIPlotLine.h"

#include "imgui.h"


namespace Coil
{
	GUIPlotLine::GUIPlotLine(const RString<> label, const Ref<std::vector<float32>> dataBuffer, float32 scaleMin, float32 scaleMax, const Ref<glm::vec2>& dimensions)
		: GUIComponent(label),
		  DataBuffer(dataBuffer),
		  ScaleMin(scaleMin),
		  ScaleMax(scaleMax),
		  Dimensions(dimensions) {}

	void GUIPlotLine::Draw() const
	{
		ImGui::PlotLines(Label->CString(), DataBuffer->_Unchecked_begin(), DataBuffer->size(), 0, nullptr, ScaleMin, ScaleMax, { Dimensions->x, Dimensions->y });
	}
}

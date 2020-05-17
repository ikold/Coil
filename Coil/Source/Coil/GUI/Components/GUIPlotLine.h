#pragma once

#include "GUIComponent.h"


namespace Coil
{
	class GUIPlotLine : public GUIComponent
	{
	public:
		GUIPlotLine(const GUIComponentProps& properties, Ref<std::vector<float32>> dataBuffer, float32 scaleMin, float32 scaleMax);

		void Draw() const override;


		void BindDataBuffer(const Ref<std::vector<float32>>& dataBuffer) { DataBuffer = dataBuffer; }

		[[nodiscard]] const Ref<std::vector<float32>>& RetrieveDataBuffer() const { return DataBuffer; }

	private:
		Ref<std::vector<float32>> DataBuffer;
		float32 ScaleMin;
		float32 ScaleMax;
	};
}

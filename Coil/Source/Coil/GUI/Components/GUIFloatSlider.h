#pragma once

#include "GUIComponent.h"


namespace Coil
{
	/**
	 * @brief GUI Float slider
	 *
	 * @note Component is bound with Ref<float32>
	 * @note Values are in range <MinValue, MaxValue>
	 */
	class GUIFloatSlider final : public GUIComponent
	{
	public:
		GUIFloatSlider(const GUIComponentProps& properties, Ref<float32> floatRef, float32 minValue = 0.f, float32 maxValue = 1.f);

		void Draw() const override;

		void SetMinValue(float32 value) { MinValue = value; }
		void SetMaxValue(float32 value) { MaxValue = value; }

		[[nodiscard]] float32 GetMinValue() const { return MinValue; }
		[[nodiscard]] float32 GetMaxValue() const { return MaxValue; }

		void BindFloatRef(const Ref<float32>& floatRef) { FloatRef = floatRef; }
		[[nodiscard]] const Ref<float32>& RetrieveFloatRef() const { return FloatRef; }

	private:
		Ref<float32> FloatRef;

		float32 MinValue;
		float32 MaxValue;
	};
}

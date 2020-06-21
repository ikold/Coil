#pragma once

#include "GUILogWindow.h"
#include "GUIComponentWindow.h"

#include "GUIOverlay.h"

#include "Components/GUIButton.h"
#include "Components/GUIText.h"
#include "Components/GUITextInput.h"
#include "Components/GUIFloatSlider.h"
#include "Components/GUIColorPicker.h"
#include "Components/GUIMultiLineTextInput.h"
#include "Components/GUIPlotLine.h"


namespace Coil
{
	class GUI
	{
	public:

		static void OnImGuiRender();

		static GUILogWindow* LogWindow(const GUIWindowProps& properties = { "Log" });

		static GUIComponentWindow* ComponentWindow(const GUIWindowProps& properties, const std::vector<Ref<GUIComponent>>& components);

		static GUIComponentWindow* ComponentWindow(const std::vector<Ref<GUIComponent>>& components = {});


		static GUIOverlay* Overlay(const GUIWindowProps& properties = {});

		static Ref<GUIButton> Button(const GUIComponentProps& properties, const std::function<void()>& callback);

		static Ref<GUIButton> Button(const std::function<void()>& callback) { return Button({}, callback); }

		static Ref<GUIText> Text(const GUIComponentProps& properties, const RString<>& stringReference);

		static Ref<GUITextInput> TextInput(const GUIComponentProps& properties, const RString<BString>& inputBuffer = BString(""));

		static Ref<GUIMultiLineTextInput> MultiLineTextInput(const GUIComponentProps& properties, const RString<BString>& inputBuffer = BString(""));

		static Ref<GUIFloatSlider> FloatSlider(const GUIComponentProps& properties, const Ref<float32>& floatRef = std::make_shared<float32>(), float32 minValue = 0.f, float32 maxValue = 1.f);

		static Ref<GUIColorPicker> ColorPicker(const GUIComponentProps& properties, const Ref<glm::vec3>& vec3Ref = std::make_shared<glm::vec3>(0));

		static Ref<GUIPlotLine> PlotLine(const GUIComponentProps& properties, const Ref<std::vector<float32>>& dataBuffer = std::make_shared<std::vector<float32>>(), float32 scaleMin = 0.f, float32 scaleMax = std::numeric_limits<float32>::max());

		static Ref<GUIText> Text(const RString<>& stringReference) { return Text({}, stringReference); }

		static Ref<GUITextInput> TextInput(const RString<BString>& inputBuffer = BString("")) { return TextInput({}, inputBuffer); }

		static Ref<GUIMultiLineTextInput> MultiLineTextInput(const RString<BString>& inputBuffer = BString("")) { return MultiLineTextInput({}, inputBuffer); }

		static Ref<GUIFloatSlider> FloatSlider(const Ref<float32>& floatRef = std::make_shared<float32>(), float32 minValue = 0.f, float32 maxValue = 1.f) { return FloatSlider({}, floatRef, minValue, maxValue); }

		static Ref<GUIColorPicker> ColorPicker(const Ref<glm::vec3>& vec3Ref = std::make_shared<glm::vec3>(0)) { return ColorPicker({}, vec3Ref); }

		static Ref<GUIPlotLine> PlotLine(const Ref<std::vector<float32>>& dataBuffer = std::make_shared<std::vector<float32>>(), float32 scaleMin = 0.f, float32 scaleMax = std::numeric_limits<float32>::max()) { return PlotLine({}, dataBuffer, scaleMin, scaleMax); }

	private:
		static PointerContainer<GUIWindow> Container;
	};
}

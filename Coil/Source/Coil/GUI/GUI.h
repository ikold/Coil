#pragma once

#include "GUILogWindow.h"
#include "GUIComponentWindow.h"

#include "GUIOverlay.h"

#include "Components/GUISameLine.h"
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

		static GUILogWindow* LogWindow(const GUIWindowProps& properties = {});

		static GUIComponentWindow* ComponentWindow(const GUIWindowProps& properties, const std::vector<Ref<GUIComponent>>& components);

		static GUIComponentWindow* ComponentWindow(const std::vector<Ref<GUIComponent>>& components = {});


		static GUIOverlay* Overlay(const GUIWindowProps& properties = {});

		
		static Ref<GUISameLine> SameLine();
		
		static Ref<GUIButton> Button(const RString<String>& label, void (*callback)());

		static Ref<GUIText> Text(const RString<String>& stringReference);

		static Ref<GUITextInput> TextInput(const RString<String>& label = "", const RString<BString>& inputBuffer = BString(""));

		static Ref<GUIMultiLineTextInput> MultiLineTextInput(const RString<String>& label = "", const RString<BString>& inputBuffer = BString(""), const Ref<glm::vec2>& dimensions = std::make_shared<glm::vec2>(0, 0));

		static Ref<GUIFloatSlider> FloatSlider(const RString<>& label = "", const Ref<float32>& floatRef = std::make_shared<float32>(), float32 minValue = 0.f, float32 maxValue = 1.f);

		static Ref<GUIColorPicker> ColorPicker(const RString<String>& label = "", const Ref<glm::vec3>& vec3Ref = std::make_shared<glm::vec3>(0));

		static Ref<GUIPlotLine> PlotLine(const RString<String>& label = "", const Ref<std::vector<float32>>& dataBuffer = std::make_shared<std::vector<float32>>(), float32 scaleMin = 0.f, float32 scaleMax = std::numeric_limits<float32>::max(), const Ref<glm::vec2>& dimensions = std::make_shared<glm::vec2>(0, 0));


		static RString<> MakeUniqueLabel(const RString<>& label);

	private:
		static PointerContainer<GUIWindow> Container;
		static int32 LabelID;
	};
}

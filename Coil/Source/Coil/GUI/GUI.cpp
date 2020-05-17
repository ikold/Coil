#include "pch.h"
#include "GUI.h"


namespace Coil
{
	PointerContainer<GUIWindow> GUI::Container;
	int32 GUI::LabelID = 0;

	void GUI::OnImGuiRender()
	{
		for (auto* window : Container)
			window->OnImGuiRender();
	}

	GUILogWindow* GUI::LogWindow(const GUIWindowProps& properties)
	{
		auto* window = new GUILogWindow(properties);
		Container.Push(window);
		return window;
	}

	GUIComponentWindow* GUI::ComponentWindow(const GUIWindowProps& properties, const std::vector<Ref<GUIComponent>>& components)
	{
		auto* window = new GUIComponentWindow(properties);
		Container.Push(window);

		for (const auto& component : components)
			window->AddComponent(component);

		return window;
	}

	GUIComponentWindow* GUI::ComponentWindow(const std::vector<Ref<GUIComponent>>& components)
	{
		return ComponentWindow({}, components);
	}

	GUIOverlay* GUI::Overlay(const GUIWindowProps& properties)
	{
		auto* window = new GUIOverlay(properties);
		Container.Push(window);
		return window;
	}

	Ref<GUISameLine> GUI::SameLine()
	{
		return std::make_shared<GUISameLine>();
	}


	Ref<GUIButton> GUI::Button(const RString<String>& label, void (* callback)())
	{
		return std::make_shared<GUIButton>(MakeUniqueLabel(label), callback);
	}

	Ref<GUIText> GUI::Text(const RString<String>& stringReference)
	{
		return std::make_shared<GUIText>(stringReference);
	}

	Ref<GUITextInput> GUI::TextInput(const RString<String>& label, const RString<BString>& inputBuffer)
	{
		return std::make_shared<GUITextInput>(MakeUniqueLabel(label), inputBuffer);
	}

	Ref<GUIMultiLineTextInput> GUI::MultiLineTextInput(const RString<String>& label, const RString<BString>& inputBuffer, const Ref<glm::vec2>& dimensions)
	{
		return std::make_shared<GUIMultiLineTextInput>(MakeUniqueLabel(label), inputBuffer, dimensions);
	}

	Ref<GUIFloatSlider> GUI::FloatSlider(const RString<>& label, const Ref<float32>& floatRef, float32 minValue, float32 maxValue)
	{
		return std::make_shared<GUIFloatSlider>(MakeUniqueLabel(label), floatRef, minValue, maxValue);
	}

	Ref<GUIColorPicker> GUI::ColorPicker(const RString<String>& label, const Ref<glm::vec3>& vec3Ref)
	{
		return std::make_shared<GUIColorPicker>(MakeUniqueLabel(label), vec3Ref);
	}

	Ref<GUIPlotLine> GUI::PlotLine(const RString<String>& label, const Ref<std::vector<float32>>& dataBuffer, float32 scaleMin, float32 scaleMax, const Ref<glm::vec2>& dimensions)
	{
		return std::make_shared<GUIPlotLine>(MakeUniqueLabel(label), dataBuffer, scaleMin, scaleMax, dimensions);
	}


	RString<> GUI::MakeUniqueLabel(const RString<>& label)
	{
		return PString("%S##%d", label->CString(), label->GetSize(), LabelID++).ToString();
	}
}

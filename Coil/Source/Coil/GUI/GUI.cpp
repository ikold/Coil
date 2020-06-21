#include "pch.h"
#include "GUI.h"


namespace Coil
{
	PointerContainer<GUIWindow> GUI::Container;


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

	void GUI::HideWindow(GUIWindow& window)
	{
		window.GetProperties().Visible = false;
	}

	void GUI::ShowWindow(GUIWindow& window)
	{
		window.GetProperties().Visible = true;
	}

	void GUI::ToggleWindowVisibility(GUIWindow& window)
	{
		window.GetProperties().Visible = !(window.GetProperties().Visible);
	}

	bool GUI::HideWindow(const RString<>& name)
	{
		GUIWindow* window = GetWindow(name);

		if (!window)
			return false;

		HideWindow(*window);
		
		return true;
	}

	bool GUI::ShowWindow(const RString<>& name)
	{
		GUIWindow* window = GetWindow(name);

		if (!window)
			return false;

		ShowWindow(*window);

		return true;
	}

	bool GUI::ToggleWindowVisibility(const RString<>& name)
	{
		GUIWindow* window = GetWindow(name);

		if (!window)
			return false;

		ToggleWindowVisibility(*window);

		return true;
	}


	GUIOverlay* GUI::Overlay(const GUIWindowProps& properties)
	{
		auto* window = new GUIOverlay(properties);
		Container.Push(window);
		return window;
	}


	Ref<GUIButton> GUI::Button(const GUIComponentProps& properties, const std::function<void()>& callback)
	{
		return std::make_shared<GUIButton>(properties, callback);
	}

	Ref<GUIText> GUI::Text(const GUIComponentProps& properties, const RString<>& stringReference)
	{
		return std::make_shared<GUIText>(properties, stringReference);
	}

	Ref<GUITextInput> GUI::TextInput(const GUIComponentProps& properties, const RString<BString>& inputBuffer)
	{
		return std::make_shared<GUITextInput>(properties, inputBuffer);
	}

	Ref<GUIMultiLineTextInput> GUI::MultiLineTextInput(const GUIComponentProps& properties, const RString<BString>& inputBuffer)
	{
		return std::make_shared<GUIMultiLineTextInput>(properties, inputBuffer);
	}

	Ref<GUIFloatSlider> GUI::FloatSlider(const GUIComponentProps& properties, const Ref<float32>& floatRef, float32 minValue, float32 maxValue)
	{
		return std::make_shared<GUIFloatSlider>(properties, floatRef, minValue, maxValue);
	}

	Ref<GUIColorPicker> GUI::ColorPicker(const GUIComponentProps& properties, const Ref<glm::vec3>& vec3Ref)
	{
		return std::make_shared<GUIColorPicker>(properties, vec3Ref);
	}

	Ref<GUIPlotLine> GUI::PlotLine(const GUIComponentProps& properties, const Ref<std::vector<float32>>& dataBuffer, float32 scaleMin, float32 scaleMax)
	{
		return std::make_shared<GUIPlotLine>(properties, dataBuffer, scaleMin, scaleMax);
	}
}

#include "pch.h"
#include "GUI.h"


namespace Coil
{
	std::vector<Ref<GUIWindow>> GUI::Container;


	void GUI::OnImGuiRender()
	{
		for (const auto& window : Container)
			window->OnImGuiRender();
	}

	Ref<GUILogWindow> GUI::LogWindow(const GUIWindowProps& properties)
	{
		auto window = CreateRef<GUILogWindow>(properties);
		Container.push_back(window);
		return window;
	}

	Ref<GUIComponentWindow> GUI::ComponentWindow(const GUIWindowProps& properties, const std::vector<Ref<GUIComponent>>& components)
	{
		auto window = CreateRef<GUIComponentWindow>(properties);
		Container.push_back(window);

		for (const auto& component : components)
			window->AddComponent(component);

		return window;
	}

	Ref<GUIComponentWindow> GUI::ComponentWindow(const std::vector<Ref<GUIComponent>>& components)
	{
		return ComponentWindow({}, components);
	}

	void GUI::HideWindow(const Ref<GUIWindow>& window)
	{
		window->GetProperties().Visible = false;
	}

	void GUI::ShowWindow(const Ref<GUIWindow>& window)
	{
		window->GetProperties().Visible = true;
	}

	void GUI::ToggleWindowVisibility(const Ref<GUIWindow>& window)
	{
		window->GetProperties().Visible = !window->GetProperties().Visible;
	}

	bool GUI::HideWindow(const RString<>& name)
	{
		const auto window = GetWindow(name);

		if (!window)
			return false;

		HideWindow(window);

		return true;
	}

	bool GUI::ShowWindow(const RString<>& name)
	{
		const auto window = GetWindow(name);

		if (!window)
			return false;

		ShowWindow(window);

		return true;
	}

	bool GUI::ToggleWindowVisibility(const RString<>& name)
	{
		const auto window = GetWindow(name);

		if (!window)
			return false;

		ToggleWindowVisibility(window);

		return true;
	}


	Ref<GUIOverlay> GUI::Overlay(const GUIWindowProps& properties)
	{
		auto window = CreateRef<GUIOverlay>(properties);
		Container.push_back(window);
		return window;
	}


	Ref<GUIButton> GUI::Button(const GUIComponentProps& properties, const std::function<void()>& callback)
	{
		return CreateRef<GUIButton>(properties, callback);
	}

	Ref<GUIText> GUI::Text(const GUIComponentProps& properties, const RString<>& stringReference)
	{
		return CreateRef<GUIText>(properties, stringReference);
	}

	Ref<GUITextInput> GUI::TextInput(const GUIComponentProps& properties, const RString<BString>& inputBuffer)
	{
		return CreateRef<GUITextInput>(properties, inputBuffer);
	}

	Ref<GUIMultiLineTextInput> GUI::MultiLineTextInput(const GUIComponentProps& properties, const RString<BString>& inputBuffer)
	{
		return CreateRef<GUIMultiLineTextInput>(properties, inputBuffer);
	}

	Ref<GUIFloatSlider> GUI::FloatSlider(const GUIComponentProps& properties, const Ref<float32>& floatRef, float32 minValue, float32 maxValue)
	{
		return CreateRef<GUIFloatSlider>(properties, floatRef, minValue, maxValue);
	}

	Ref<GUIColorPicker> GUI::ColorPicker(const GUIComponentProps& properties, const Ref<glm::vec4>& vec4Ref)
	{
		return CreateRef<GUIColorPicker>(properties, vec4Ref);
	}

	Ref<GUIPlotLine> GUI::PlotLine(const GUIComponentProps& properties, const Ref<std::vector<float32>>& dataBuffer, float32 scaleMin, float32 scaleMax)
	{
		return CreateRef<GUIPlotLine>(properties, dataBuffer, scaleMin, scaleMax);
	}
}

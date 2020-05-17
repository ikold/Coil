#pragma once

#include "GUILogWindow.h"
#include "GUIComponentWindow.h"

#include "GUIOverlay.h"

#include "Components/GUIText.h"
#include "Components/GUITextInput.h"
#include "Components/GUIColorPicker.h"


namespace Coil
{
	class GUI
	{
	public:

		static void OnImGuiRender()
		{
			for (auto* window : Container)
				window->OnImGuiRender();
		}

		static GUILogWindow* LogWindow(const GUIWindowProps& properties = {})
		{
			auto* window = new GUILogWindow(properties);
			Container.Push(window);
			return window;
		}

		static GUIComponentWindow* ComponentWindow(const GUIWindowProps& properties, const std::vector<Ref<GUIComponent>>& components)
		{
			auto* window = new GUIComponentWindow(properties);
			Container.Push(window);

			for (const auto& component : components)
				window->AddComponent(component);

			return window;
		}

		static GUIComponentWindow* ComponentWindow(const std::vector<Ref<GUIComponent>>& components = {})
		{
			return ComponentWindow({}, components);
		}


		static GUIOverlay* Overlay(const GUIWindowProps& properties = {})
		{
			auto* window = new GUIOverlay(properties);
			Container.Push(window);
			return window;
		}


		static Ref<GUIText> Text(const RString<String>& stringReference)
		{
			return std::make_shared<GUIText>(stringReference);
		}

		static Ref<GUITextInput> TextInput(const RString<String>& label = "", const RString<BString>& inputBuffer = BString(""))
		{
			return std::make_shared<GUITextInput>(MakeUniqueLabel(label), inputBuffer);
		}

		static Ref<GUIColorPicker> ColorPicker(const RString<String>& label = "", const Ref<glm::vec3>& vec3Ref = std::make_shared<glm::vec3>(1.f))
		{
			return std::make_shared<GUIColorPicker>(MakeUniqueLabel(label), vec3Ref);
		}


		static RString<> MakeUniqueLabel(const RString<>& label)
		{
			return PString("%S##%d", label->CString(), label->GetSize(), LabelID++).ToString();
		}

	private:
		static PointerContainer<GUIWindow> Container;

		static int32 LabelID;
	};
}

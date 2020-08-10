#pragma once

#include "GUIWindow.h"
#include "Components/GUIComponent.h"


namespace Coil
{
	/**
	 * @brief GUI window for drawing components
	 *
	 * @note Example of simplifier creation defined in GUI utility class: \n
	 * GUI::ComponentWindow({ "Window name" }, {\n
	 *   GUI::Text("Example text component"),\n
	 *   GUI::ColorPicker({ "", -1 }, Vec4Ref)\n
	 * }
	 */
	class GUIComponentWindow final : public GUIWindow
	{
	public:
		explicit GUIComponentWindow(const GUIWindowProps& properties);

		void AddComponent(const Ref<GUIComponent>& component) { Components.push_back(component); }

	protected:
		void Draw() const override;

	private:
		std::vector<Ref<GUIComponent>> Components{};
	};
}

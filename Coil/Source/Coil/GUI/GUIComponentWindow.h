#pragma once

#include "GUIWindow.h"
#include "Components/GUIComponent.h"

namespace Coil
{
	class GUIComponentWindow final : public GUIWindow
	{
	public:
		GUIComponentWindow(const GUIWindowProps& properties);

		void AddComponent(const Ref<GUIComponent>& component) { Components.push_back(component); }
		
	protected:
		void Draw() const override;

		void SetUp() const override {};
	private:
		std::vector<Ref<GUIComponent>> Components{};
	};
}
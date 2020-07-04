#pragma once

#include "GUIComponent.h"


namespace Coil
{
	class GUIColorPicker final : public GUIComponent
	{
	public:
		explicit GUIColorPicker(const GUIComponentProps& properties, Ref<glm::vec4> vec4Ref);

		void Draw() const override;

		void BindVec3(const Ref<glm::vec4>& ref) { Vec4Ref = ref; }

		[[nodiscard]] const Ref<glm::vec4>& RetrieveVec3Ref() const { return Vec4Ref; }

	private:
		Ref<glm::vec4> Vec4Ref;
	};
}

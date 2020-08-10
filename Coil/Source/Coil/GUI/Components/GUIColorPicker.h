#pragma once

#include "GUIComponent.h"


namespace Coil
{
	/**
	 * @brief GUI Color picker
	 *
	 * @note Component is bound with Ref<glm::vec4>
	 */
	class GUIColorPicker final : public GUIComponent
	{
	public:
		explicit GUIColorPicker(const GUIComponentProps& properties, Ref<glm::vec4> vec4Ref);

		void Draw() const override;

		void BindVec4(const Ref<glm::vec4>& ref) { Vec4Ref = ref; }
		[[nodiscard]] const Ref<glm::vec4>& RetrieveVec4Ref() const { return Vec4Ref; }

	private:
		Ref<glm::vec4> Vec4Ref;
	};
}

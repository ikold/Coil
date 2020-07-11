#include "pch.h"
#include "GUIColorPicker.h"

#include "imgui.h"
#include  <glm/gtc/type_ptr.hpp>


namespace Coil
{
	GUIColorPicker::GUIColorPicker(const GUIComponentProps& properties, Ref<glm::vec4> vec4Ref)
		: GUIComponent(properties),
		  Vec4Ref(Move(vec4Ref)) {}

	void GUIColorPicker::Draw() const
	{
		CL_PROFILE_FUNCTION_HIGH()

		ImGui::ColorEdit4(Properties.Label->CString(), value_ptr(*Vec4Ref));
	}
}

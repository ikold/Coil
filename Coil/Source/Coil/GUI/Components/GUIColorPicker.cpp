#include "pch.h"
#include "GUIColorPicker.h"

#include "imgui.h"
#include  <glm/gtc/type_ptr.hpp>


namespace Coil
{
	GUIColorPicker::GUIColorPicker(const GUIComponentProps& properties, const Ref<glm::vec3>& vec3Ref)
		: GUIComponent(properties),
		  Vec3Ref(vec3Ref) {}

	void GUIColorPicker::Draw() const
	{
		ImGui::ColorEdit3(Properties.Label->CString(), value_ptr(*Vec3Ref));
	}
}

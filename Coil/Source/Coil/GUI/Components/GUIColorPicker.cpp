#include "pch.h"
#include "GUIColorPicker.h"

#include "imgui.h"
#include  <glm/gtc/type_ptr.hpp>


namespace Coil
{
	GUIColorPicker::GUIColorPicker(const RString<String>& label, const Ref<glm::vec3>& vec3Ref)
		: GUIComponent(label),
		  Vec3Ref(vec3Ref) {}

	void GUIColorPicker::Draw() const
	{
		ImGui::ColorEdit3(Label->CString(), value_ptr(*Vec3Ref));
	}
}

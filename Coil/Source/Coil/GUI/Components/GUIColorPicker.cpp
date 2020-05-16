#include "pch.h"
#include "GUIColorPicker.h"

#include "imgui.h"
#include  <glm/gtc/type_ptr.hpp>


namespace Coil
{
	GUIColorPicker::GUIColorPicker(const RString<String>& name, const Ref<glm::vec3>& vec3Ref)
		: Name(name),
		  Vec3Ref(vec3Ref) {}

	void GUIColorPicker::Draw() const
	{
		ImGui::ColorEdit3(Name->CString(), glm::value_ptr(*Vec3Ref));
	}
}

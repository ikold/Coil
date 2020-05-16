#pragma once

#include "GUIComponent.h"

#include <glm/glm.hpp>


namespace Coil
{
	class GUIColorPicker : public GUIComponent
	{
	public:
		explicit GUIColorPicker(const RString<String>& name, const Ref<glm::vec3>& vec3Ref);

		void Draw() const override;

		void BindVec3(const Ref<glm::vec3>& ref) { Vec3Ref = ref; }

		[[nodiscard]] Ref<glm::vec3> RetrieveVec3Ref() const { return Vec3Ref; }
		
	private:
		RString<String> Name;
		Ref<glm::vec3> Vec3Ref;
	};
}

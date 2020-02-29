#pragma once

#include "Coil/Core.h"

#include "imgui.h"

namespace Coil
{
	class COIL_API ImGuiWindow
	{
	public:
		ImGuiWindow(const char* name, int width, int height);
		~ImGuiWindow();

		void OnUpdate() const;

		virtual void Draw() const = 0;
	private:
		const char* Name;
		int Width;
		int Height;
	};

}


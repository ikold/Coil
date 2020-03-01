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

	protected:
		int TextTopCulling() const;
		int TextBottomCulling(int linesTotal) const;
		int WindowSizeInLines() const;

		virtual void Draw() const = 0;

	protected:
		bool* OpenFlag;
		ImGuiWindowFlags Flags;

	private:
		const char* Name;
		int Width;
		int Height;

		float fontHeight = 17.f;
		float dummyDefaultOffset = 4.f;
	};

}


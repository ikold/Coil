#pragma once

#include "Coil/Core.h"

#include "imgui.h"

namespace Coil
{
	class COIL_API ImGuiWindow
	{
	public:
		ImGuiWindow(const char8* name, int32 width, int32 height);
		~ImGuiWindow();

		void OnUpdate() const;

	protected:
		int32 TextTopCulling() const;
		int32 TextBottomCulling(int32 linesTotal) const;
		int32 WindowSizeInLines() const;

		virtual void Draw() const = 0;

	protected:
		bool* OpenFlag;
		ImGuiWindowFlags Flags;

	private:
		const char8* Name;
		int32 Width;
		int32 Height;

		float32 fontHeight = 17.f;
		float32 dummyDefaultOffset = 4.f;
	};

}


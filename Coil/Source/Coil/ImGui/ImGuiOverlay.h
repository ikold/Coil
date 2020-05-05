#pragma once

#include "Coil/Core.h"

#include "ImGuiWindow.h"

namespace Coil
{
	class ImGuiOverlay : public ImGuiWindow
	{
	public:
		ImGuiOverlay(RString<String> name, int32 width, int32 height);
		~ImGuiOverlay();

		inline void BindTextBuffer(RString<String> reference) { StringReference = reference; }

	protected:
		void Draw() const override;

	private:
		RString<String> StringReference;
	};
}

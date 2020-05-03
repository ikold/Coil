#pragma once

#include "Coil/Core.h"

#include "ImGuiWindow.h"

namespace Coil
{
	class COIL_API ImGuiOverlay : public ImGuiWindow
	{
	public:
		ImGuiOverlay(RString name, int32 width, int32 height);
		~ImGuiOverlay();

		void BindTextBuffer(RString reference) { StringReference = reference; }

	protected:
		void Draw() const override;

	private:
		RString StringReference;
	};
}

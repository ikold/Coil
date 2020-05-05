#pragma once

#include "Coil/Core.h"

#include "ImGuiWindow.h"

namespace Coil
{
	class COIL_API ImGuiLogWindow : public ImGuiWindow
	{
	public:
		ImGuiLogWindow(RString<String> name, int32 width, int32 height)
			: ImGuiWindow(name, width, height), Buffer(nullptr)
		{}

		~ImGuiLogWindow();

		void Draw() const override;

		inline void BindBuffer(PointerContainer<Log>* buffer) { Buffer = buffer; }

	private:
		PointerContainer<Log>* Buffer;
	};
}

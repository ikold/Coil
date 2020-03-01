#pragma once

#include "Coil/Core.h"

#include "ImGuiWindow.h"

namespace Coil
{
	class COIL_API ImGuiLogWindow : public ImGuiWindow
	{
	public:
		ImGuiLogWindow(const char* name, int width, int height)
			: ImGuiWindow(name, width, height), Buffer(nullptr)
		{
		}
		~ImGuiLogWindow();

		void Draw() const override;

		void BindBuffer(PointerContainer<Log>* buffer) { Buffer = buffer; }

	private:
		PointerContainer<Log>* Buffer;
	};

}


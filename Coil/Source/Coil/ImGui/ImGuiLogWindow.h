#pragma once


#include <utility>


#include "ImGuiWindow.h"


namespace Coil
{
	class ImGuiLogWindow : public ImGuiWindow
	{
	public:
		ImGuiLogWindow(RString<String> name, int32 width, int32 height)
			: ImGuiWindow(std::move(name), width, height),
			  Buffer(nullptr)
		{}

		~ImGuiLogWindow();

		void Draw() const override;

		void BindBuffer(PointerContainer<Log>* buffer) { Buffer = buffer; }

	private:
		PointerContainer<Log>* Buffer;
	};
}

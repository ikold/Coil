#pragma once

#include <utility>


#include "ImGuiWindow.h"


namespace Coil
{
	class ImGuiOverlay final : public ImGuiWindow
	{
	public:
		ImGuiOverlay(const RString<String>& name, int32 width, int32 height);
		~ImGuiOverlay() = default;

		void BindTextBuffer(const RString<String>& reference) { StringReference = reference; }

	protected:
		void Draw() const override;

	private:
		RString<String> StringReference;
	};
}

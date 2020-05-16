#pragma once

#include <utility>


#include "GUIWindow.h"


namespace Coil
{
	class GUIOverlay final : public GUIWindow
	{
	public:
		GUIOverlay(const GUIWindowProps& properties);
		~GUIOverlay() = default;

		void BindTextBuffer(const RString<String>& reference) { StringReference = reference; }

	protected:
		void SetUp() const override;
		
		void Draw() const override;

	private:
		RString<String> StringReference;
	};
}

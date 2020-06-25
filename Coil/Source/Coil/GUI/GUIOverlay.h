#pragma once

#include "GUIWindow.h"


namespace Coil
{
	class GUIOverlay final : public GUIWindow
	{
	public:
		GUIOverlay(const GUIWindowProps& properties);
		~GUIOverlay() = default;

		void BindTextBuffer(const RString<>& reference) { StringReference = reference; }

	protected:
		void SetUp() const override;

		void Draw() const override;

	private:
		RString<> StringReference;
	};
}

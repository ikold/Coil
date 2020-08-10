#pragma once

#include "GUIWindow.h"


namespace Coil
{
	/**
	 * @brief Gui overlay for drawing bound text
	 */
	class GUIOverlay final : public GUIWindow
	{
	public:
		explicit GUIOverlay(const GUIWindowProps& properties);

		void BindTextBuffer(const RString<>& reference) { StringReference = reference; }

	protected:
		void SetUp() const override;
		void Draw() const override;

	private:
		RString<> StringReference;
	};
}

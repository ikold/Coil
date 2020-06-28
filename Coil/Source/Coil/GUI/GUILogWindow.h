#pragma once


#include "GUIWindow.h"


namespace Coil
{
	class GUILogWindow final : public GUIWindow
	{
	public:
		explicit GUILogWindow(const GUIWindowProps& properties);

		void Draw() const override;

		void BindBuffer(PointerContainer<Log>* buffer) { Buffer = buffer; }

	private:
		PointerContainer<Log>* Buffer;
	};
}

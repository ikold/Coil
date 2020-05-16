#pragma once


#include "GUIWindow.h"


namespace Coil
{
	class GUILogWindow : public GUIWindow
	{
	public:
		GUILogWindow(const GUIWindowProps& properties);

		void Draw() const override;

		void BindBuffer(PointerContainer<Log>* buffer) { Buffer = buffer; }

	protected:
		void SetUp() const override {};
	private:
		PointerContainer<Log>* Buffer;
	};
}

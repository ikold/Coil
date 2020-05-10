#pragma once

#include "ImGuiWindow.h"
#include "Coil/Utilities/PointerCointainer.h"

namespace Coil
{
	class ImGuiInterface
	{
	public:
		template<typename T>
		static typename std::enable_if<std::is_base_of<ImGuiWindow, T>::value, T&>::type
			Create(RString<String> name = "", int32 width = 512, int32 height = 256)
		{
			T* window = new T(name, width, height);
			Container.Push(window);
			return *window;
		}

		static void OnImGuiRender()
		{
			for (auto* window : Container)
				window->OnImGuiRender();
		}

	private:
		static PointerContainer<ImGuiWindow> Container;
	};
}

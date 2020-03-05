#pragma once

#include "Coil/Core.h"

#include "ImGuiWindow.h"
#include "Coil/PointerCointainer.h"

namespace Coil
{
	class COIL_API ImGuiInterface
	{
	public:
		template<typename T>
		static typename std::enable_if<std::is_base_of<ImGuiWindow, T>::value, T&>::type
		Creat(const char8* name = "", int32 width = 512, int32 height = 256)
		{
			T* window = new T(name, width, height);
			Container.Push(window);
			return *window;
		}

		static void OnUpdate()
		{
			for (auto window : Container)
				window->OnUpdate();
		}

	private:
		static PointerContainer<ImGuiWindow> Container;
	};

}


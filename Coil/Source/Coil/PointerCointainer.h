#pragma once

#include "Coil/Core.h"
#include <vector>

namespace Coil
{
	template<typename T>
	class COIL_API PointerContainer
	{
	public:
		PointerContainer() {}
		~PointerContainer()
		{
			for (auto element : Container)
				delete element;
		}

		void Push(T* ptr) { Container.push_back(ptr); }
		T* Pop() { Container.pop_back(); }

		typename std::vector<T*>::iterator begin() { return Container.begin(); }
		typename std::vector<T*>::iterator end() { return Container.end(); }

	private:
		std::vector<T*> Container;
	};
}
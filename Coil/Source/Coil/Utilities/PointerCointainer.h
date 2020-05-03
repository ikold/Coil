#pragma once

#include "Coil/Core.h"
#include <vector>

namespace Coil
{
	template<typename T>
	class COIL_API PointerContainer
	{
	public:
		PointerContainer()
		{}

		~PointerContainer()
		{
			for (auto element : Container)
				delete element;
		}

		void Push(T* ptr) { Container.push_back(ptr); }
		T* Pop() { Container.pop_back(); }

		inline typename std::vector<T*>::iterator begin() { return Container.begin(); }
		inline typename std::vector<T*>::iterator end() { return Container.end(); }

		inline int32 size() { return (int32)Container.size(); }

	private:
		std::vector<T*> Container;
	};
}
#pragma once


namespace Coil
{
	template<typename T>
	class PointerContainer
	{
	public:
		PointerContainer() = default;

		~PointerContainer()
		{
			for (auto element : Container)
				delete element;
		}

		void Push(T* ptr) { Container.push_back(ptr); }
		T* Pop() { return Container.pop_back(); }

		typename std::vector<T*>::iterator begin() { return Container.begin(); }
		typename std::vector<T*>::iterator end() { return Container.end(); }

		int32 size() { return static_cast<int32>(Container.size()); }

	private:
		std::vector<T*> Container;
	};
}

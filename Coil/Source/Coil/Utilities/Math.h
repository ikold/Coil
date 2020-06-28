#pragma once


namespace Coil
{
	class Math
	{
	public:
		template<typename T>
		static T Truncate(T value, T min, T max)
		{
			if (value < min)
				return min;

			if (value > max)
				return max;

			return value;
		}

		template<typename T>
		static T WrapCycle(T value, T cycle, T min, T max)
		{
			while (value < min)
				value += cycle;

			while (value > max)
				value -= cycle;

			return value;
		}
	};
}

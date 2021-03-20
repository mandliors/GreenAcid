#pragma once

#include <random>
#include <ctime>
#include <algorithm>

namespace ox {
	
	class Random
	{
		friend class GameWindow;

	public:
		Random() = delete;

		static int Range(int min, int max /*inclusive, exclusive*/);
		static float Float();
		
		template<typename Iter>
		static void Shuffle(Iter begin, const Iter end)
		{
			std::shuffle(begin, end, s_Generator);
		}
		template<typename T>
		static void Shuffle(T* container)
		{
			Shuffle(std::begin(*container), std::end(*container));
		}

	private:
		static void __Init();

	private:
		static std::mt19937 s_Generator;
		static std::uniform_real_distribution<float> s_RandomFloat;
	};
}
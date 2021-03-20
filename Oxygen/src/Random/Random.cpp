#include "Random.h"

namespace ox {

	std::mt19937 Random::s_Generator = std::mt19937();
	std::uniform_real_distribution<float> Random::s_RandomFloat;

	void Random::__Init()
	{
		s_Generator.seed(std::time(0));
		s_RandomFloat = std::uniform_real_distribution<float>(0.0f, 1.0f);
	}

	int Random::Range(int min, int max /*inclusive, exclusive*/)
	{
		std::uniform_int_distribution<int> random(min, max - 1);
		return random(s_Generator);
	}

	float Random::Float()
	{
		return s_RandomFloat(s_Generator);
	}

}
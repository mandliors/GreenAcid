#pragma once

#include <glm/glm.hpp>
#include <math.h>
#include <vector>

#define PI 3.14159265
#define TWO_PI 6.283185307
#define HALF_PI 1.570796326
#define QUARTER_PI 0.785398163

namespace ox
{
	typedef glm::vec2 Vec2;
	typedef glm::vec3 Vec3;
	typedef glm::vec4 Vec4;
	typedef glm::mat3 Mat3;
	typedef glm::mat4 Mat4;

	struct Math
	{
		Math() = delete;
		~Math() = default;
		
		template<typename... Args>
		static float Min(Args&&... args)
		{
			std::vector<float> numbers = { static_cast<float>(args)... };
			if (numbers.size() == 0)
				return 0.0f;
			float min = numbers[0];
			for (uint32_t i = 1; i < numbers.size(); i++)
			{
				if (numbers[i] < min)
					min = numbers[i];
			}
			return min;
		}
		template<typename... Args>
		static float Max(Args&&... args)
		{
			std::vector<float> numbers = { static_cast<float>(args)... };
			if (numbers.size() == 0)
				return 0.0f;
			float max = numbers[0];
			for (uint32_t i = 1; i < numbers.size(); i++)
			{
				if (numbers[i] > max)
					max = numbers[i];
			}
			return max;
		}
		static double Abs(double value);
		static double Sin(double value);
		static double Cos(double value);
		static double Tan(double value);
		static double Asin(double value);
		static double Acos(double value);
		static double Atan(double value);
		static double Atan2(double a, double b);
		static double Log(double value);
		static double Map(double value, double min, double max, double from, double to);
		static double Clamp(double t, double x, double y);
		static double Constrain(double t, double x, double y);
		static double ToDegrees(double value);
		static double ToRadians(double value);
		static ox::Vec2 Lerp(const Vec2& a, const Vec2& b, double t);
		static ox::Vec3 Lerp(const Vec3& a, const Vec3& b, double t);
	};
}
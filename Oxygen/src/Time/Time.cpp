#include "Time.h"
#include <iostream>

namespace ox {

#pragma region Time
	float Time::s_DeltaTime = 0;
	std::map<std::string, int> Time::s_ReferenceTimes = {};

	void Time::Bind(const std::string& id, int referenceTimeMillis)
	{
		s_ReferenceTimes.insert_or_assign(id, referenceTimeMillis);
	}
	void Time::Unbind(const std::string& id)
	{
		s_ReferenceTimes.erase(id);
	}
	void Time::SetReferenceTime(const std::string& id, int referenceTimeMillis)
	{
		s_ReferenceTimes.find(id)->second = referenceTimeMillis;
	}
	int Time::GetElapsedMillis()
	{
		return (int)(glfwGetTime() * 1000);
	}
	int Time::GetElapsedMillis(const std::string& id)
	{
		return  (int)(glfwGetTime() * 1000) - s_ReferenceTimes.find(id)->second;
	}
	int Time::GetElapsedSeconds()
	{
		return  (int)(glfwGetTime());
	}
	int Time::GetElapsedSeconds(const std::string& id)
	{
		return  (int)(glfwGetTime()) - s_ReferenceTimes.find(id)->second / 1000;
	}

	void Time::Delay(int milliseconds)
	{
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
		long long now = start;
		while (milliseconds > (float)(now - start) * 0.001f) now = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}

#pragma endregion

#pragma region Timer
	Timer::Timer(const char* name)
		: m_Name(name), m_Stopped(false) { }
	void Timer::Start()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}
	void Timer::Stop()
	{
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
		float duration = (end - start) * 0.001f;

		m_Stopped = true;

		std::cout << m_Name << ": " << duration << "ms" << std::endl;
	}
#pragma endregion
}
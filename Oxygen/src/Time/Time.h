#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <chrono>

namespace ox {

	struct Time
	{
		friend class GameWindow;
		
		Time() = delete;
		~Time() = default;

		static void Bind(const std::string& id, int referenceTimeMillis = glfwGetTime() * 1000);
		static void Unbind(const std::string& id);
		static void SetReferenceTime(const std::string& id, int referenceTimeMillis = glfwGetTime() * 1000);
		static int GetElapsedMillis();
		static int GetElapsedMillis(const std::string& id);
		static int GetElapsedSeconds();
		static int GetElapsedSeconds(const std::string& id);
		static float GetDeltaTime() { return s_DeltaTime; }
		static float GetDeltaTimeInMillis() { return s_DeltaTime * 1000.0f; }
		static void Delay(int milliseconds);
		
	private:
		static std::map<std::string, int> s_ReferenceTimes;
		static float s_DeltaTime;
	};

	class Timer
	{
	public:
		Timer(const char* name);
		~Timer() = default;
		void Start();
		void Stop();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
		const char* m_Name;
		bool m_Stopped;
	};

	template<typename Fn>
	class ScopedTimer
	{
	public:
		ScopedTimer(const char* name, Fn&& fn)
			: m_Name(name), m_Stopped(false), m_Function(fn)
		{
			m_StartTime = std::chrono::high_resolution_clock::now();
		}
		~ScopedTimer()
		{
			if (!m_Stopped)
				Stop();
		}
		void Stop()
		{
			std::chrono::steady_clock::time_point endTime = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
			float duration = (end - start) * 0.001f;

			m_Stopped = true;

			m_Function(m_Name, duration);
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
		const char* m_Name;
		bool m_Stopped;
		Fn m_Function;
	};
}
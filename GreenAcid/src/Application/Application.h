#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include "Oxygen.h"

using namespace ox;

namespace GreenAcid {

	using AppDescriptor = ox::WindowDescriptor;
	
	class Application
	{
	public:
		Application() = delete;
		Application(const Application&) = delete;

		static void Init(AppDescriptor& descriptor);
		static void Run();
		static bool IsRunning();
		static void Close();

		static float GetFPS();
		static glm::vec2 GetPosition();
		static uint32_t GetWidth();
		static uint32_t GetHeight();

		static Pointer<ox::GameWindow> GetOxygenWindow();

	private:
		static void OnStart();
		static void OnUpdate(float deltaTime);
		static void OnImGuiRender();
		static void OnShutdown();

	private:
		static Pointer<ox::GameWindow> s_Window;
		static AppDescriptor s_Descriptor;
		static Pointer<ox::OrthographicCameraController> s_Camera;
		static Pointer<ox::Framebuffer> s_Framebuffer;
		static Pointer<ox::Scene> s_ActiveScene;
		static glm::vec2 s_ViewportSize;
		static bool s_ViewportFocused;
		static bool s_ViewportHovered;
	};
}
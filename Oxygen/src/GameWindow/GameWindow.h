#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

namespace ox {

	class OpenGLContext;

	struct WindowDescriptor
	{
		std::string Title = "Oxygen";
		uint32_t Width = 1280;
		uint32_t Height = 720;
		int32_t X = -1;
		int32_t Y = -1;
		std::string IconPath;
		float MaxFPS = 60.0f;
		bool VSync = true;

		void (*Start)() = nullptr;
		void (*Update)(float deltaTime) = nullptr;
		void (*Shutdown)() = nullptr;
	};

	class GameWindow
	{
		friend class EventManager;
		friend class Input;
		friend class ImGuiLayer;
		friend class VertexArray;

	public:
		GameWindow(const WindowDescriptor& descriptor);
		GameWindow(const GameWindow&) = delete;

		void MainLoop();
		bool IsOpen() const;
		void Close();

		float GetFPS();
		glm::vec2 GetPosition() const;
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

		GLFWwindow* GetNativeWindow() const;

	public:
		static GameWindow* Get();
	
	private:
		static GameWindow* s_Instance;
		GLFWwindow* m_Handle;
		WindowDescriptor m_Descriptor;
		OpenGLContext* m_Context;
		bool m_Open;
		float m_LastFrametime;
		bool m_Minimized;
	};
}
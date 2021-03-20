#pragma once

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "../Events/Event.h"

namespace ox {


	class ImGuiLayer
	{
		friend class GameWindow;
		friend class EventManager;

	public:
		ImGuiLayer(const ImGuiLayer&) = delete;
		static void BlockEvents(bool value);

	private:
		ImGuiLayer() { }
		static void __Init();
		static void __Update();
		static void __Render();
		static void __HandleEvent(Event& event);
		static void __Shutdown();


		static float s_Time;
		static bool s_BlockEvents;
	};
}
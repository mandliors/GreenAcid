#pragma once

#include "../Base/KeyCodes.h"
#include "../Base/MouseButtonCodes.h"
#include "../Math/Math.h"
#include "../Events/Event.h"
#include <map>

namespace ox {

	class Input
	{
		friend class GameWindow;
		friend class EventManager;
		friend class ImGuiLayer;

	public:
		Input(const Input&) = delete;

		static bool GetKeyDown(int keycode);
		static bool GetKey(int keycode);
		static bool GetKeyUp(int keycode);
		static bool GetMouseButtonDown(int button);
		static bool GetMouseButton(int button);
		static bool GetMouseButtonUp(int button);
		static ox::Vec2 GetMousePosition();
		static ox::Vec2 GetPreviousMousePosition();
		static ox::Vec2 GetMouseScrollDelta();

	private:
		Input() { }
		static void __Init();
		static void __Update();
		static void __HandleEvent(const Event& event);
		static void __ResetStates() { Input::__Init(); }
		static int __KeyAt(const std::map<int, int>& map, int index);

		static bool s_PreviousKeyState[GLFW_KEY_COUNT], s_KeyState[GLFW_KEY_COUNT];
		static bool s_PreviousMouseButtonState[GLFW_MOUSEBUTTON_COUNT], s_MouseButtonState[GLFW_MOUSEBUTTON_COUNT];
		static ox::Vec2 s_PreviousMousePosition;
		static ox::Vec2 s_MousePosition;
		static ox::Vec2 s_MouseScrollDelta;
	};
}
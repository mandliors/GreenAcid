#include "Input.h"
#include "GameWindow/GameWindow.h"
#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include <cstring>

namespace ox {

	bool Input::s_PreviousKeyState[GLFW_KEY_COUNT], Input::s_KeyState[GLFW_KEY_COUNT];
	bool Input::s_PreviousMouseButtonState[GLFW_MOUSEBUTTON_COUNT], Input::s_MouseButtonState[GLFW_MOUSEBUTTON_COUNT];
	ox::Vec2 Input::s_PreviousMousePosition;
	ox::Vec2 Input::s_MousePosition;
	ox::Vec2 Input::s_MouseScrollDelta;

	bool Input::GetKeyDown(int keycode)
	{
		return s_KeyState[keycode] && !s_PreviousKeyState[keycode];
	}

	bool Input::GetKey(int keycode)
	{
		return s_KeyState[keycode];
	}

	bool Input::GetKeyUp(int keycode)
	{
		return !s_KeyState[keycode] && s_PreviousKeyState[keycode];
	}

	bool Input::GetMouseButtonDown(int button)
	{
		return s_MouseButtonState[button] && !s_PreviousMouseButtonState[button];
	}

	bool Input::GetMouseButton(int button)
	{
		return s_MouseButtonState[button];
	}

	bool Input::GetMouseButtonUp(int button)
	{
		return !s_MouseButtonState[button] && s_PreviousMouseButtonState[button];
	}

	ox::Vec2 Input::GetMousePosition()
	{
		return s_MousePosition;
	}

	ox::Vec2 Input::GetPreviousMousePosition()
	{
		return s_PreviousMousePosition;
	}

	ox::Vec2 Input::GetMouseScrollDelta()
	{
		return s_MouseScrollDelta;
	}

	void Input::__Init()
	{
		//Set key and mouse button states
		memset(s_KeyState, false, sizeof(s_KeyState));
		memset(s_KeyState, false, sizeof(s_PreviousKeyState));
		memset(s_MouseButtonState, false, sizeof(s_MouseButtonState));
		memset(s_MouseButtonState, false, sizeof(s_PreviousMouseButtonState));
		
		//Mouse position
		double xpos, ypos;
		glfwGetCursorPos(GameWindow::s_Instance->m_Handle, &xpos, &ypos);
		s_MousePosition.x = (float)xpos;
		s_MousePosition.y = (float)ypos;
		s_PreviousMousePosition = s_MousePosition;
	}

	void Input::__Update()
	{
		//Key and mouse button states
		memcpy(s_PreviousKeyState, s_KeyState, sizeof(s_PreviousKeyState));
		memcpy(s_PreviousMouseButtonState, s_MouseButtonState, sizeof(s_PreviousMouseButtonState));
		
		//Mouse position
		s_PreviousMousePosition = s_MousePosition;

		//Mouse scroll delta
		s_MouseScrollDelta.x = 0;
		s_MouseScrollDelta.y = 0;
	}

	void Input::__HandleEvent(const Event& event)
	{
		switch (event.GetEventType())
		{
			case EventType::KeyPressed:
			{
				s_KeyState[static_cast<const KeyReleasedEvent&>(event).GetKeyCode()] = true;
				break;
			}
			case EventType::KeyReleased:
			{
				s_KeyState[static_cast<const KeyReleasedEvent&>(event).GetKeyCode()] = false;
				break;
			}
			case EventType::MouseButtonPressed:
			{
				s_MouseButtonState[static_cast<const MouseButtonPressedEvent&>(event).GetMouseButton()] = true;
				break;
			}
			case EventType::MouseButtonReleased:
			{
				s_MouseButtonState[static_cast<const MouseButtonReleasedEvent&>(event).GetMouseButton()] = false;
				break;
			}
			case EventType::MouseMoved:
			{
				const MouseMovedEvent& mouseEv = static_cast<const MouseMovedEvent&>(event);
				s_MousePosition.x = mouseEv.GetX();
				s_MousePosition.y = mouseEv.GetY();
				break;
			}
			case EventType::MouseScrolled:
			{
				const MouseScrolledEvent& mouseEv = static_cast<const MouseScrolledEvent&>(event);
				s_MouseScrollDelta.x = mouseEv.GetXOffset();
				s_MouseScrollDelta.y = mouseEv.GetYOffset();
				break;
			}
		}
	}

	int Input::__KeyAt(const std::map<int, int>& map, int index)
	{
		std::map<int, int>::const_iterator iter = map.begin();
		std::advance(iter, index);
		return iter->first;
	}
}
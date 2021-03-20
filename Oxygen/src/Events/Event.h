#pragma once

#include <iostream>
#include <functional>

namespace ox {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowMinimise, WindowUnminimise,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	class Event
	{	
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool Handled = false;
	};
}
#pragma once

#include "Event.h"
#include <sstream>

namespace ox {

	class KeyEvent : public Event
	{
	public:
		int GetKeyCode() const { return m_Keycode; }
	
	protected:
		KeyEvent(int keycode)
			: m_Keycode(keycode) { }

		int m_Keycode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, bool repeated)
			: KeyEvent(keycode), m_Repeated(repeated) { }

		bool IsRepeated() const { return m_Repeated; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_Keycode << " (repeated: " << m_Repeated << ")";
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyPressed; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyPressed"; }

	private:
		bool m_Repeated;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_Keycode;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyReleased; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyReleased"; }
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_Keycode;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyTyped; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyTyped"; }
	};
}
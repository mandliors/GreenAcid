#pragma once

#include "Event.h"
#include <sstream>

namespace ox {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) { }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::WindowResize; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowResize"; }

	private:
		uint32_t m_Width, m_Height;
	};

	class WindowMinimiseEvent : public Event
	{
	public:
		WindowMinimiseEvent() { }

		static EventType GetStaticType() { return EventType::WindowMinimise; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowMinimise"; }
	};

	class WindowUnminimiseEvent : public Event
	{
	public:
		WindowUnminimiseEvent() { }

		static EventType GetStaticType() { return EventType::WindowUnminimise; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowUnminimise"; }
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() { }

		static EventType GetStaticType() { return EventType::WindowClose; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowClose"; }
	};
}
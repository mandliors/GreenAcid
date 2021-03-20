#pragma once

#include "Event.h"
#include <sstream>

namespace ox {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_PosX(x), m_PosY(y) { }

		float GetX() const { return m_PosX; }
		float GetY() const { return m_PosY; }


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_PosX << ", " << m_PosY;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseMoved; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseMoved"; }

	private:
		float m_PosX, m_PosY;

	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_OffsetX(xOffset), m_OffsetY(yOffset) { }

		float GetXOffset() const { return m_OffsetX; }
		float GetYOffset() const { return m_OffsetY; }


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_OffsetX << ", " << m_OffsetY;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseScrolled; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseScrolled"; }

	private:
		float m_OffsetX, m_OffsetY;

	};

	class MouseButtonEvent : public Event
	{
	public:
		int GetMouseButton() const { return m_Button; }

	protected:
		MouseButtonEvent(int button)
			: m_Button(button) { }

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseButtonPressed; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseButtonPressed"; }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseButtonReleased; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseButtonReleased"; }
	};
}
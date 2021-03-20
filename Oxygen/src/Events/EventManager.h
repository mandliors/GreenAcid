#pragma once

#include "Event.h"
#include "WindowEvent.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "../Input/Input.h"
#include "../Debug/Debug.h"
#include <queue>
#include <functional>

namespace ox {

	class EventManager
	{
		friend class GameWindow;

	public:
		static void DispatchEvent(EventType type, const std::function<void(const Event&)>& func);

	private:
		EventManager() { }
		static void __Init();
		template<typename T, typename... Args>
		static void __EmplaceEvent(Args&&... args)
		{
			EventManager::s_EventQueue.emplace(std::make_unique<T>(std::forward<Args>(args)...));
		}
		static void __HandleEvents();

	private:
		struct EventDispatcher
		{
			EventDispatcher(EventType type, const std::function<void(const Event&)>& function)
				: Type(type), Function(function) { }

			EventType Type;
			std::function<void(const Event&)> Function;
		};

		static std::queue<std::unique_ptr<Event>> s_EventQueue;
		static std::vector<EventDispatcher> s_EventDispatchers;
	};

}
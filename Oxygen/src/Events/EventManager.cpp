#include "EventManager.h"
#include "../GameWindow/GameWindow.h"
#include "../ImGui/ImGuiLayer.h"
#include "../Renderer/OrthographicCameraController.h"

namespace ox {

	std::queue<std::unique_ptr<Event>> EventManager::s_EventQueue;
	std::vector<EventManager::EventDispatcher> EventManager::s_EventDispatchers;

	void EventManager::DispatchEvent(EventType type, const std::function<void(const Event&)>& func)
	{
		s_EventDispatchers.emplace_back(type, func);
	}

	void EventManager::__Init() { }

	void EventManager::__HandleEvents()
	{
		Input::__Update();
		while (!s_EventQueue.empty())
		{
			Event& event = *s_EventQueue.front();
			ImGuiLayer::__HandleEvent(event);
			if (!event.Handled)
			{
				Input::__HandleEvent(event);
				OrthographicCameraController::__HandleEvent(event);

				//Check event type
				switch (event.GetEventType())
				{
					case EventType::WindowResize:
					{
						WindowResizeEvent& winEv = static_cast<WindowResizeEvent&>(event);
						glViewport(0, 0, (GLsizei)winEv.GetWidth(), (GLsizei)winEv.GetHeight());
						Input::__ResetStates();
						break;
					}
					case EventType::WindowMinimise:
					{
						GameWindow::s_Instance->m_Minimized = true;
						break;
					}
					case EventType::WindowUnminimise:
					{
						GameWindow::s_Instance->m_Minimized = false;
						break;
					}
					case EventType::WindowClose:
					{
						GameWindow::s_Instance->m_Open = false;
						break;
					}
				}

				//Check the event dispatchers
				for (const auto& dispatcher : s_EventDispatchers)
				{
					if (dispatcher.Type == event.GetEventType())
						dispatcher.Function(event);
				}
			}

			s_EventQueue.pop();
		}
	}

}
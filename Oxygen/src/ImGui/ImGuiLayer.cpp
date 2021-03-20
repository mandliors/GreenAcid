#include "ImGuiLayer.h"
#include "../GameWindow/GameWindow.h"
#include "../Base/KeyCodes.h"
#include "../Base/MouseButtonCodes.h"
#include "../Events/WindowEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"
#include "../Input/Input.h"

namespace ox {

	float ImGuiLayer::s_Time = 0.0f;
	bool ImGuiLayer::s_BlockEvents = false;

	void ImGuiLayer::BlockEvents(bool value)
	{
		s_BlockEvents = value;
	}

	void ImGuiLayer::__Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(GameWindow::s_Instance->m_Handle, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::__Update()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::__Render()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)GameWindow::s_Instance->GetWidth(), (float)GameWindow::s_Instance->GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* currentContextBackup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(currentContextBackup);
		}
	}

	void ImGuiLayer::__HandleEvent(Event& event)
	{
		switch (event.GetEventType())
		{
			case EventType::WindowResize:
			{
				const WindowResizeEvent& winEv = static_cast<const WindowResizeEvent&>(event);
				ImGuiIO& io = ImGui::GetIO();
				io.DisplaySize = ImVec2((float)winEv.GetWidth(), (float)winEv.GetHeight());
				io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
				break;
			}
			case EventType::KeyPressed:
			{
				ImGuiIO& io = ImGui::GetIO();
				io.KeysDown[Input::__KeyAt(KEYCODE_DEFINITIONS, static_cast<const KeyPressedEvent&>(event).GetKeyCode())] = true;
				io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
				io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
				io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
				io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
				break;
			}
			case EventType::KeyReleased:
			{
				ImGuiIO& io = ImGui::GetIO();
				io.KeysDown[Input::__KeyAt(KEYCODE_DEFINITIONS, static_cast<const KeyPressedEvent&>(event).GetKeyCode())] = false;
				break;
			}
			/*case EventType::KeyTyped:
			{
				KeyTypedEvent& keyEv = static_cast<KeyTypedEvent&>(event);
				ImGuiIO& io = ImGui::GetIO();
				int keycode = keyEv.GetKeyCode();
				if (keycode > 0 && keycode < 0x10000)
					io.AddInputCharacter((unsigned short)keycode);
				break;
			}*/
			case EventType::MouseButtonPressed:
			{
				ImGuiIO& io = ImGui::GetIO();
				io.MouseDown[Input::__KeyAt(MOUSEBUTTON_DEFINITIONS, static_cast<const MouseButtonPressedEvent&>(event).GetMouseButton())] = true;
				break;
			}
			case EventType::MouseButtonReleased:
			{
				ImGuiIO& io = ImGui::GetIO();
				io.MouseDown[Input::__KeyAt(MOUSEBUTTON_DEFINITIONS, static_cast<const MouseButtonReleasedEvent&>(event).GetMouseButton())] = false;
				break;
			}
			case EventType::MouseMoved:
			{
				const MouseMovedEvent& mouseEv = static_cast<const MouseMovedEvent&>(event);
				ImGuiIO& io = ImGui::GetIO();
				io.MousePos = ImVec2(mouseEv.GetX(), mouseEv.GetY());
				break;
			}
			case EventType::MouseScrolled:
			{
				const MouseScrolledEvent& mouseEv = static_cast<const MouseScrolledEvent&>(event);
				ImGuiIO& io = ImGui::GetIO();
				io.MouseWheelH += mouseEv.GetXOffset();
				io.MouseWheel += mouseEv.GetYOffset();
				if (s_BlockEvents) event.Handled |= 1 & io.WantCaptureMouse;
				break;
			}
		}
	}

	void ImGuiLayer::__Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
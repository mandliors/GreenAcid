#include "GameWindow.h"
#include "Base/Base.h"
#include "Base/Defines.h"
#include "Base/KeyCodes.h"
#include "Base/MouseButtonCodes.h"
#include "Renderer/OpenGLContext.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Events/EventManager.h"
#include "Input/Input.h"
#include "Time/Time.h"
#include "Debug/Debug.h"
#include "Random/Random.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/ImGuiLayer.h"
#include "Vendor/stb_image/stb_image.h"

namespace ox {

	GameWindow* GameWindow::s_Instance = nullptr;
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		std::cerr << "GLFW Error: " << error << ": " << description << std::endl;
	}

#ifdef OPENGL_DEBUG_CALLBACK
	void GLAPIENTRY MessageCallback( GLenum source,
									 GLenum type,
									 GLuint id,
									 GLenum severity,
									 GLsizei length,
									 const GLchar* message,
									 const void* userParam )
	{
  		fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
	}
#endif

	GameWindow::GameWindow(const WindowDescriptor& descriptor)
	{
		OX_ASSERT(!s_Instance, "GameWindow class was already instantiated");
		s_Instance = this;

		//Window data
		m_Minimized = false;
		m_Descriptor = descriptor;

		//Initialize GLFW
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			OX_ASSERT(success, "GLFW failed to initialize");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
	#ifdef OPENGL_45_OR_MORE
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#else
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#endif

		//GLFW window
		m_Handle = glfwCreateWindow((int)m_Descriptor.Width, (int)m_Descriptor.Height, m_Descriptor.Title.c_str(), NULL, NULL);
		if (m_Descriptor.X != -1 && m_Descriptor.Y != -1)
			glfwSetWindowPos(m_Handle, (int)m_Descriptor.X, (int)m_Descriptor.Y);
		if (!m_Descriptor.IconPath.empty())
		{
			GLFWimage images[1];
			images[0].pixels = stbi_load(m_Descriptor.IconPath.c_str(), &images[0].width, &images[0].height, 0, 4); 
			glfwSetWindowIcon(m_Handle, 1, images);
			stbi_image_free(images[0].pixels);
		}

		//OpenGL context
		m_Context = new OpenGLContext(m_Handle);
		m_Context->Init();

		//Window properties
		glfwSetWindowUserPointer(m_Handle, &m_Descriptor);
		if (m_Descriptor.VSync) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		//GLFW callbacks
		glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* window, int width, int height)
		{
			WindowDescriptor& data = *(WindowDescriptor*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			glViewport(0, 0, width, height);
			//Minimize
			if (width == 0 || height == 0)
				EventManager::__EmplaceEvent<WindowMinimiseEvent>();
			//Resize
			else
			{
				if (s_Instance->m_Minimized) EventManager::__EmplaceEvent<WindowUnminimiseEvent>();
				EventManager::__EmplaceEvent<WindowResizeEvent>(width, height);
			}
		});
		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* window)
		{
			EventManager::__EmplaceEvent<WindowCloseEvent>();
		});
		glfwSetKeyCallback(m_Handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			switch (action)
			{
				case GLFW_PRESS:
				{
					EventManager::__EmplaceEvent<KeyPressedEvent>(KEYCODE_DEFINITIONS.at(key), false);
					break;
				}
				case GLFW_REPEAT:
				{
					EventManager::__EmplaceEvent<KeyPressedEvent>(KEYCODE_DEFINITIONS.at(key), true);
					break;
				}
				case GLFW_RELEASE:
				{
					EventManager::__EmplaceEvent<KeyReleasedEvent>(KEYCODE_DEFINITIONS.at(key));
					break;
				}
			}
		});
		glfwSetCharCallback(m_Handle, [](GLFWwindow* window, unsigned int keycode)
		{
			EventManager::__EmplaceEvent<KeyTypedEvent>(keycode);
		});
		glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* window, int button, int action, int mods)
		{
			switch (action)
			{
				case GLFW_PRESS:
				{
					EventManager::__EmplaceEvent<MouseButtonPressedEvent>(MOUSEBUTTON_DEFINITIONS.at(button));
					break;
				}
				case GLFW_RELEASE:
				{
					EventManager::__EmplaceEvent<MouseButtonReleasedEvent>(MOUSEBUTTON_DEFINITIONS.at(button));
					break;
				}
			}
		});
		glfwSetScrollCallback(m_Handle, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			EventManager::__EmplaceEvent<MouseScrolledEvent>((float)xOffset, (float)yOffset);
		});
		glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* window, double xPos, double yPos)
		{
			EventManager::__EmplaceEvent<MouseMovedEvent>((float)xPos, (float)yPos);
		});

#ifdef OPENGL_DEBUG_CALLBACK
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback( MessageCallback, 0 );
#endif

		//Initialize other classes
		EventManager::__Init();
		Input::__Init();
		Renderer::__Init();
		ImGuiLayer::__Init();
		Random::__Init();

		if (m_Descriptor.Start) m_Descriptor.Start();
	}

	void GameWindow::MainLoop()
	{
		m_Open = true;

		while (m_Open)
		{
			float time = (float)glfwGetTime();
			float dt = time - m_LastFrametime;
			static const float frameTime = 1.0f / m_Descriptor.MaxFPS;
			if (dt < frameTime)
			{
				Time::Delay((int)((frameTime - dt) * 1000.0f));
				dt = frameTime;
			}
			Time::s_DeltaTime = dt;
			m_LastFrametime = time;

			if (m_Minimized)
			{
				while (m_Minimized)
				{
					EventManager::__HandleEvents();
					glfwPollEvents();
					Time::Delay(50);
				}
			}

			glfwPollEvents();
			EventManager::__HandleEvents();
			ImGuiLayer::__Update();

			if (m_Descriptor.Update) m_Descriptor.Update(dt);

			ImGuiLayer::__Render();
			m_Context->SwapBuffers();
		}

		if (m_Descriptor.Shutdown) m_Descriptor.Shutdown();

		Renderer::__Shutdown();
		ImGuiLayer::__Shutdown();

		delete m_Context;

		//GLFW
		glfwDestroyWindow(m_Handle);
		glfwTerminate();
	}

	bool GameWindow::IsOpen() const
	{
		return m_Open;
	}
	
	void GameWindow::Close()
	{
		m_Open = false;
	}

	float GameWindow::GetFPS()
	{
		return 1.0f / Time::s_DeltaTime;
	}

	glm::vec2 GameWindow::GetPosition() const
	{
		return { m_Descriptor.X, m_Descriptor.Y };
	}


	uint32_t GameWindow::GetWidth() const
	{
		return m_Descriptor.Width;
	}

	uint32_t GameWindow::GetHeight() const
	{
		return m_Descriptor.Height;
	}

	GLFWwindow* GameWindow::GetNativeWindow() const
	{
		return m_Handle;
	}

	GameWindow* GameWindow::Get()
	{
		return s_Instance;
	}
}

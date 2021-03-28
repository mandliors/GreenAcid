#include "OpenGLContext.h"
#include "Base/Base.h"
#include "Debug/Debug.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ox {

	OpenGLContext::OpenGLContext(GLFWwindow* handle)
		: m_Handle(handle)
	{
		OX_ASSERT(handle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_Handle);
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		OX_ASSERT(success, "GLAD failed to initialize");

		Debug::Log("OpenGL Renderer: ", glGetString(GL_VENDOR), " ", glGetString(GL_RENDERER), " ", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Handle);
	}

}
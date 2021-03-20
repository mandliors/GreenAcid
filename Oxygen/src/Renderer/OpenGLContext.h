#pragma once

struct GLFWwindow;

namespace ox {

	class OpenGLContext
	{
	public:
		OpenGLContext(GLFWwindow* handle);

		void Init();
		void SwapBuffers();

	private:
		GLFWwindow* m_Handle;
	};

}
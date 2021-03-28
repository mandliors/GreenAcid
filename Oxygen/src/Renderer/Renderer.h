#pragma once

#include "Base/Base.h"
#include "VertexArray.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace ox {

	class Renderer
	{
		friend class GameWindow;

	public:
		static void Clear(float r, float g, float b, float a);
		static void EnableWireframe(bool enable);

		static void BeginScene(const Pointer<OrthographicCamera>& camera);
		static void EndScene();

		static void Submit(const Pointer<Shader>& shader, const Pointer<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawIndexed(const Pointer<VertexArray>& vertexArray, uint32_t indexCount = 0);
	
	private:
		Renderer() { }
		~Renderer() { }
		static void __Init();
		static void __Shutdown();

	private:
		static glm::mat4 m_ProjectionViewMatrix;
	};
}
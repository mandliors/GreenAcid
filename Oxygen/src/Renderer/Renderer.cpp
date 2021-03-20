#include "Renderer.h"
#include "Renderer2D.h"

namespace ox {

	glm::mat4 Renderer::m_ProjectionViewMatrix;

	void Renderer::__Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

		Renderer2D::__Init();
	}

	void Renderer::__Shutdown()
	{
		Renderer2D::__Shutdown();
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::EnableWireframe(bool enable)
	{
		if (enable) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Renderer::BeginScene(const Pointer<OrthographicCamera>& camera)
	{
		m_ProjectionViewMatrix = camera->GetProjectionViewMatrix();
	}

	void Renderer::EndScene() { }

	void Renderer::Submit(const Pointer<Shader>& shader, const Pointer<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ProjectionViewMatrix", m_ProjectionViewMatrix);
		shader->UploadUniformMat4("u_ModelMatrix", transform);
		vertexArray->Bind();
		DrawIndexed(vertexArray);
	}

	void Renderer::DrawIndexed(const Pointer<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
#include "VertexArray.h"
#include "../GameWindow/GameWindow.h"
#include <glad/glad.h>

namespace ox {

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	Pointer<VertexArray> VertexArray::Create()
	{
		return std::make_shared<VertexArray>();
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(Pointer<VertexBuffer>& vertexBuffer)
	{
		OX_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer has no layout");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (int i = 0; i < layout.GetElements().size(); i++)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, layout.GetElements()[i].GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(layout.GetElements()[i].Type),
				layout.GetElements()[i].Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)layout.GetElements()[i].Offset);
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(Pointer<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}
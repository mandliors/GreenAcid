#include "Base/Base.h"
#include "Base/Defines.h"
#include "Buffer.h"
#include <glad/glad.h>

namespace ox {

	///////////////////////////////////////////////////////////////////////
	//                           VertexBuffer                            //
	///////////////////////////////////////////////////////////////////////
	VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
	{
	#ifdef OPENGL_45_OR_MORE
		glCreateBuffers(1, &m_RendererID);
	#else
		glGenBuffers(1, &m_RendererID);
	#endif
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(uint32_t size)
	{
	#ifdef OPENGL_45_OR_MORE
		glCreateBuffers(1, &m_RendererID);
	#else
		glGenBuffers(1, &m_RendererID);
	#endif
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	Pointer<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return MakePointer<ox::VertexBuffer>(vertices, size);
	}

	Pointer<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		return MakePointer<ox::VertexBuffer>(size);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	///////////////////////////////////////////////////////////////////////
	//                           IndexBuffer                             //
	///////////////////////////////////////////////////////////////////////
	IndexBuffer::IndexBuffer(uint32_t* vertices, uint32_t count)
		: m_Count(count)
	{
	#ifdef OPENGL_45_OR_MORE
		glCreateBuffers(1, &m_RendererID);
	#else
		glGenBuffers(1, &m_RendererID);
	#endif
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), vertices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	Pointer<IndexBuffer> IndexBuffer::Create(uint32_t* vertices, uint32_t count)
	{
		return MakePointer<ox::IndexBuffer>(vertices, count);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
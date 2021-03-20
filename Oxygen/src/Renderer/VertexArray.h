#pragma once

#include "../Base/Base.h"
#include "Buffer.h"

namespace ox {

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		static Pointer<VertexArray> Create();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(Pointer<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(Pointer<IndexBuffer>& indexBuffer);

		const std::vector<Pointer<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		const Pointer<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<Pointer<VertexBuffer>> m_VertexBuffers;
		Pointer<IndexBuffer> m_IndexBuffer;
	};

}
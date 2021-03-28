#pragma once

#include "Base/Base.h"
#include <glad/glad.h>
#include <vector>
#include <string>

namespace ox {

	enum class ShaderDataType
	{
		None = 0, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat3, Mat4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:	   return 8;
			case ShaderDataType::Int3:	   return 12;
			case ShaderDataType::Int4:	   return 16;
			case ShaderDataType::Float:	   return 4;
			case ShaderDataType::Float2:   return 8;
			case ShaderDataType::Float3:   return 12;
			case ShaderDataType::Float4:   return 16;
			case ShaderDataType::Mat3:	   return 36;
			case ShaderDataType::Mat4:	   return 64;
			case ShaderDataType::Bool:	   return 1;
		}

		OX_ASSERT(false, "Unknown OpenGLShaderDataType");
		return 0;
	}

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ox::ShaderDataType::Int:     return GL_INT;
			case ox::ShaderDataType::Int2:	  return GL_INT;
			case ox::ShaderDataType::Int3:	  return GL_INT;
			case ox::ShaderDataType::Int4:	  return GL_INT;
			case ox::ShaderDataType::Float:	  return GL_FLOAT;
			case ox::ShaderDataType::Float2:  return GL_FLOAT;
			case ox::ShaderDataType::Float3:  return GL_FLOAT;
			case ox::ShaderDataType::Float4:  return GL_FLOAT;
			case ox::ShaderDataType::Mat3:	  return GL_FLOAT;
			case ox::ShaderDataType::Mat4:	  return GL_FLOAT;
			case ox::ShaderDataType::Bool:	  return GL_BOOL;
		}

		OX_ASSERT(false, "Unknown OpenGLShaderDataType");
		return 0;
	}

	struct BufferElement
	{
		ShaderDataType Type;
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() { }

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) { }

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Int:      return 1;
				case ShaderDataType::Int2:	   return 2;
				case ShaderDataType::Int3:	   return 3;
				case ShaderDataType::Int4:	   return 4;
				case ShaderDataType::Float:	   return 1;
				case ShaderDataType::Float2:   return 2;
				case ShaderDataType::Float3:   return 3;
				case ShaderDataType::Float4:   return 4;
				case ShaderDataType::Mat3:	   return 9;
				case ShaderDataType::Mat4:	   return 16;
				case ShaderDataType::Bool:	   return 1;
			}

			OX_ASSERT(false, "Unknown OpenGLShaderDataType");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() { }

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};


	class VertexBuffer
	{
	public:
		VertexBuffer(uint32_t size);
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		static Pointer<VertexBuffer> Create(uint32_t size);
		static Pointer<VertexBuffer> Create(float* vertices, uint32_t size);

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32_t size);

		const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};


	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* vertices, uint32_t count);
		~IndexBuffer();

		static Pointer<IndexBuffer> Create(uint32_t* vertices, uint32_t count);

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}

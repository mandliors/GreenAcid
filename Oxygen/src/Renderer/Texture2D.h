#pragma once

#include <glad/glad.h>
#include "Base/Base.h"
#include <string>
#include <memory>

namespace ox {

	enum class TextureFiltering
	{
		NEAREST = 0x2600, LINEAR = 0x2601
	};

	class Texture2D
	{
		friend class Renderer2D;

	public:
		Texture2D(const std::string& path, TextureFiltering minificationFiltering = TextureFiltering::LINEAR, TextureFiltering magnificationFiltering = TextureFiltering::NEAREST);
		Texture2D(uint32_t width, uint32_t height, void* data, TextureFiltering minificationFiltering = TextureFiltering::LINEAR, TextureFiltering magnificationFiltering = TextureFiltering::NEAREST);
		~Texture2D();

		static Pointer<Texture2D> Create(uint32_t width, uint32_t height, void* data, TextureFiltering minificationFiltering = TextureFiltering::LINEAR, TextureFiltering magnificationFiltering = TextureFiltering::NEAREST);
		static Pointer<Texture2D> Create(const std::string& path, TextureFiltering minificationFiltering = TextureFiltering::LINEAR, TextureFiltering magnificationFiltering = TextureFiltering::NEAREST);

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetRendererID() const { return m_RendererID; }

		void Bind(uint32_t slot = 0) const;

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

}
#include "Texture2D.h"
#include "../Base/Base.h"
#include <stb_image/stb_image.h>

namespace ox {

	Texture2D::Texture2D(const std::string& path, TextureFiltering minificationFiltering, TextureFiltering magnificationFiltering)
	{
		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		OX_ASSERT(data != nullptr, "Failed to load image from " + path);
		m_Width = (uint32_t)width;
		m_Height = (uint32_t)height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else
			OX_ASSERT(false, "Format of file " + path + " is not supported");

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, (GLint)minificationFiltering);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, (GLint)magnificationFiltering);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	Texture2D::Texture2D(uint32_t width, uint32_t height, void* data, TextureFiltering minificationFiltering, TextureFiltering magnificationFiltering)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, (GLint)minificationFiltering);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, (GLint)magnificationFiltering);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	Pointer<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void* data, TextureFiltering minificationFiltering, TextureFiltering magnificationFiltering)
	{
		return MakePointer<Texture2D>(width, height, data, minificationFiltering, magnificationFiltering);
	}

	Pointer<Texture2D> Texture2D::Create(const std::string& path, TextureFiltering minificationFiltering, TextureFiltering magnificationFiltering)
	{
		return MakePointer<Texture2D>(path, minificationFiltering, magnificationFiltering);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}
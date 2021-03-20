#pragma once

#include "Texture2D.h"
#include <glm/glm.hpp>

namespace ox {

	class SubTexture2D
	{
	public:
		SubTexture2D(const Pointer<Texture2D>& texture, float x1, float y1, float x2, float y2);
		
		static Pointer<SubTexture2D> CreateFromCoords(const Pointer<Texture2D>& texture, float x, float y, float cellWidth, float cellHeight, float spriteWidth = 1.0f, float spriteHeight = 1.0f);

		const Pointer<Texture2D>& GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

	private:
		const Pointer<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};
}
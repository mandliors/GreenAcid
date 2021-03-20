#include "SubTexture2D.h"
#include "Texture2D.h"

namespace ox {

	SubTexture2D::SubTexture2D(const Pointer<Texture2D>& texture, float x1, float y1, float x2, float y2)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { x1, y1 };
		m_TexCoords[1] = { x2, y1 };
		m_TexCoords[2] = { x2, y2 };
		m_TexCoords[3] = { x1, y2 };
	}

	Pointer<SubTexture2D> SubTexture2D::CreateFromCoords(const Pointer<Texture2D>& texture, float x, float y, float cellWidth, float cellHeight, float spriteWidth, float spriteHeight)
	{
		float x1 = (x * cellWidth) / (float)texture->GetWidth();
		float y1 = (y * cellHeight) / (float)texture->GetHeight();
		float x2 = ((x + spriteWidth) * cellWidth) / (float)texture->GetWidth();
		float y2 = ((y + spriteHeight) * cellHeight) / (float)texture->GetHeight();
		return MakePointer<SubTexture2D>(texture, x1, y1, x2, y2);
	}
}
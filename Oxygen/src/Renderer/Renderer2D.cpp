#include "../Base/Base.h"
#include "Renderer2D.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "SubTexture2D.h"
#include "VertexArray.h"
#include <glm/gtc/matrix_transform.hpp>
#include <array>

namespace ox {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};
	
	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Pointer<VertexArray> QuadVertexArray;
		Pointer<VertexBuffer> QuadVertexBuffer;
		Pointer<Shader> TextureShader;
		Pointer<Texture2D> WhitePixel;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Pointer<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 is WhitePixel

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::__Init()
	{
		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout(
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Pointer<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		uint32_t whitePixelData = 0xffffffff;
		s_Data.WhitePixel = Texture2D::Create(1, 1, &whitePixelData);

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhitePixel;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::__Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginScene(const Pointer<OrthographicCamera>& camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->UploadUniformMat4("u_ProjectionViewMatrix", camera->GetProjectionViewMatrix());

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0) return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		Renderer::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::NextBatch()
	{
		EndScene();
		StartBatch();
	}

	void Renderer2D::EnableWireframe(bool enable)
	{
		Renderer::EnableWireframe(enable);
	}

	//FillQuad
	void Renderer2D::DrawQuad(float x, float y, float z, float w, float h, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x, y, z })
			* glm::scale(glm::mat4(1.0f), { w, h, 1.0f });
		
		DrawQuad(transform, color);
	}
	void Renderer2D::DrawQuad(float x, float y, float w, float h, const glm::vec4& color)
	{
		DrawQuad(x, y, 0.0f, w, h, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		OX_ASSERT(s_Data.QuadVertexBufferPtr != nullptr, "A new batch hasn't been started. Have you called ox::Renderer2D::BeginScene?");

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float textureIndex = 0; //White pixel
		const float tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	//FillRotatedQuad
	void Renderer2D::DrawRotatedQuad(float x, float y, float z, float w, float h, float r, const glm::vec4& color)
	{
		OX_ASSERT(s_Data.QuadVertexBufferPtr != nullptr, "A new batch hasn't been started. Have you called ox::Renderer2D::BeginScene?");

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x, y, z })
			* glm::rotate(glm::mat4(1.0f), r, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { w, h, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawRotatedQuad(float x, float y, float w, float h, float r, const glm::vec4& color)
	{
		DrawRotatedQuad(x, y, 0.0f, w, h, r, color);
	}

	//RenderTexture
	void Renderer2D::DrawTexture(float x, float y, float z, float w, float h, const Pointer<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x, y, z })
			* glm::scale(glm::mat4(1.0f), { w, h, 1.0f });
		
		DrawTexture(transform, texture, tintColor, tilingFactor);
	}
	void Renderer2D::DrawTexture(float x, float y, float w, float h, const Pointer<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(x, y, 0.0f, w, h, texture, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(float x, float y, float z, float w, float h, const Pointer<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x, y, z })
			* glm::scale(glm::mat4(1.0f), { w, h, 1.0f });

		DrawTexture(transform, subTexture, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(float x, float y, float w, float h, const Pointer<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawTexture(x, y, 0.0f, w, h, subTexture, tintColor, tilingFactor);
	}

	void Renderer2D::DrawTexture(const glm::mat4& transform, const Pointer<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor)
	{
		OX_ASSERT(s_Data.QuadVertexBufferPtr != nullptr, "A new batch hasn't been started. Have you called ox::Renderer2D::BeginScene?");

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i]->m_RendererID == texture->m_RendererID)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawTexture(const glm::mat4& transform, const Pointer<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor)
	{
		OX_ASSERT(s_Data.QuadVertexBufferPtr != nullptr, "A new batch hasn't been started. Have you called ox::Renderer2D::BeginScene?");

		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const Pointer<Texture2D>& texture = subTexture->GetTexture();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i]->m_RendererID == texture->m_RendererID)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	//RenderRotatedTexture
	void Renderer2D::DrawRotatedTexture(float x, float y, float z, float w, float h, float r, const Pointer<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor)
	{
		OX_ASSERT(s_Data.QuadVertexBufferPtr != nullptr, "A new batch hasn't been started. Have you called ox::Renderer2D::BeginScene?");

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i]->m_RendererID == texture->m_RendererID)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x, y, z })
			* glm::rotate(glm::mat4(1.0f), r, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { w, h, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawRotatedTexture(float x, float y, float w, float h, float r, const Pointer<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawRotatedTexture(x, y, 0.0f, w, h, r, texture, tintColor, tilingFactor);
	}

	void Renderer2D::DrawRotatedTexture(float x, float y, float z, float w, float h, float r, const Pointer<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor)
	{
		OX_ASSERT(s_Data.QuadVertexBufferPtr != nullptr, "A new batch hasn't been started. Have you called ox::Renderer2D::BeginScene?");

		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const Pointer<Texture2D>& texture = subTexture->GetTexture();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i]->m_RendererID == texture->m_RendererID)
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { x, y, z })
			* glm::rotate(glm::mat4(1.0f), r, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { w, h, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotatedTexture(float x, float y, float w, float h, float r, const Pointer<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor)
	{
		DrawRotatedTexture(x, y, 0.0f, w, h, r, subTexture, tintColor);
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}
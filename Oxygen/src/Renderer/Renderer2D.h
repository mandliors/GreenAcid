#pragma once

#include "../Base/Base.h"
#include "OrthographicCamera.h"

namespace ox {

	class Texture2D;
	class SubTexture2D;

	class Renderer2D
	{
		friend class Renderer;

	public:
		static void BeginScene(const Pointer<OrthographicCamera>& camera);
		static void EndScene();
		static void Flush();

		static void EnableWireframe(bool enable);
		static void SetCoordinateSystemOrigin(CoordinateSystemOrigin origin) { s_Origin = origin; }
		static CoordinateSystemOrigin GetCoordinateSystemOrigin() { return s_Origin; }

		//Primitives
		static void DrawQuad(float x, float y, float z, float w, float h, const glm::vec4& color);
		static void DrawQuad(float x, float y, float w, float h, const glm::vec4& color);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawTexture(const glm::mat4& transform, const Pointer<Texture2D>& texture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, float tilingFactor = 1.0f);
		static void DrawTexture(const glm::mat4& transform, const Pointer<SubTexture2D>& subTexture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, float tilingFactor = 1.0f);

		static void DrawRotatedQuad(float x, float y, float z, float w, float h, float r, const glm::vec4& color);
		static void DrawRotatedQuad(float x, float y, float w, float h, float r, const glm::vec4& color);

		static void DrawTexture(float x, float y, float z, float w, float h, const Pointer<Texture2D>& texture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, float tilingFactor = 1.0f);
		static void DrawTexture(float x, float y, float w, float h, const Pointer<Texture2D>& texture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, float tilingFactor = 1.0f);
		static void DrawTexture(float x, float y, float z, float w, float h, const Pointer<SubTexture2D>& subTexture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, float tilingFactor = 1.0f);
		static void DrawTexture(float x, float y, float w, float h, const Pointer<SubTexture2D>& subTexture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, float tilingFactor = 1.0f);

		static void DrawRotatedTexture(float x, float y, float z, float w, float h, float r, const Pointer<Texture2D>& texture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, float tilingFactor = 1.0f);
		static void DrawRotatedTexture(float x, float y, float w, float h, float r, const Pointer<Texture2D>& texture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, float tilingFactor = 1.0f);
		static void DrawRotatedTexture(float x, float y, float z, float w, float h, float r, const Pointer<SubTexture2D>& subTexture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, float tilingFactor = 1.0f);
		static void DrawRotatedTexture(float x, float y, float w, float h, float r, const Pointer<SubTexture2D>& subTexture, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }, float tilingFactor = 1.0f);

		//Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static Statistics GetStats();
		static void ResetStats();

	private:
		static void __Init();
		static void __Shutdown();
		static void StartBatch();
		static void NextBatch();

	private:
		static glm::vec2 s_Offset;
		static CoordinateSystemOrigin s_Origin;
	};
}
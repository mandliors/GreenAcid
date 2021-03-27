#pragma once

#include "../Base/Base.h"
#include <glm/glm.hpp>

namespace ox {

	struct OrthographicCameraBounds
	{
		float Left;
		float Right;
		float Bottom;
		float Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	enum class CoordinateSystemOrigin
	{
		CENTER_SCREEN = 0, TOP_LEFT_CORNER
	};

	class OrthographicCamera
	{
		friend class OrthographicCameraController;
		friend class Renderer2D;

	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		
		static Pointer<OrthographicCamera> Create(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }
		OrthographicCameraBounds& GetBounds() { return m_Bounds; }
		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

		void SetPosition(const glm::vec3& position);
		void SetRotation(float rotation);
		void Resize(float aspectRatio, float zoomLevel);
		void Resize(float width, float height, float zoomLevel);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }

	private:
		void __RecalculateProjection(float left, float right, float bottom, float top);
		void __RecalculateProjectionViewMatrix();
		OrthographicCameraBounds __GetRealBounds() { return { m_Bounds.Left / m_Zoomlevel, m_Bounds.Right / m_Zoomlevel, m_Bounds.Bottom / m_Zoomlevel , m_Bounds.Top / m_Zoomlevel }; }

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionViewMatrix;
		OrthographicCameraBounds m_Bounds;

		glm::vec3 m_Position;
		float m_Rotation = 0.0f;
		float m_Zoomlevel = 1.0f;
	};
}
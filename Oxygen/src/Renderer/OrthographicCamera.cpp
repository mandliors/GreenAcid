#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ox {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_Bounds({ left, right, bottom, top }), m_ViewMatrix(1.0f), m_Position({ 0.0f, 0.0f, 0.0f })
	{
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	Pointer<OrthographicCamera> OrthographicCamera::Create(float left, float right, float bottom, float top)
	{
		return MakePointer<OrthographicCamera>(left, right, bottom, top);
	}

	void OrthographicCamera::__RecalculateProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::Resize(float aspectRatio, float zoomLevel)
	{
		m_Bounds = { -aspectRatio * zoomLevel, aspectRatio * zoomLevel, -zoomLevel, zoomLevel };
		__RecalculateProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	void OrthographicCamera::Resize(float width, float height, float zoomLevel)
	{
		m_Zoomlevel = zoomLevel;
		m_Bounds = { -width * 0.5f * zoomLevel, width * 0.5f * zoomLevel, -height * 0.5f * zoomLevel, height * 0.5f * zoomLevel };
		__RecalculateProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		__RecalculateProjectionViewMatrix();
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		m_Rotation = rotation;
		__RecalculateProjectionViewMatrix();
	}

	void OrthographicCamera::__RecalculateProjectionViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
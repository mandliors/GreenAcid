#pragma once

#include "OrthographicCamera.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"
#include <glm/glm.hpp>
#include <vector>

namespace ox {

	class OrthographicCameraController
	{
		friend class EventManager;

	public:
		OrthographicCameraController(float aspectRatio, bool canRotate = false, float moveSpeed = 1.0f, float rotationSpeed = 180.0f);
		OrthographicCameraController(float width, float height, bool canRotate = false, float moveSpeed = 400.0f, float rotationSpeed = 180.0f);

		static Pointer<OrthographicCameraController> Create(float aspectRatio, bool canRotate = false, float moveSpeed = 1.0f, float rotationSpeed = 180.0f);
		static Pointer<OrthographicCameraController> Create(float width, float height, bool canRotate = false, float moveSpeed = 400.0f, float rotationSpeed = 180.0f);

		void Update(float deltaTime);

		const Pointer<OrthographicCamera>& GetCamera() const { return m_Camera; }
		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float zoom) { m_ZoomLevel = zoom; m_Camera->Resize(m_Bounds.GetWidth(), m_Bounds.GetHeight(), m_ZoomLevel); }

		OrthographicCameraBounds& GetBounds() { return m_Camera->GetBounds(); }
		const OrthographicCameraBounds& GetBounds() const { return m_Camera->GetBounds(); }

		void Resize(float aspectRatio);
		void Resize(float width, float height);

	private:
		float m_ZoomLevel;
		OrthographicCameraBounds m_Bounds;
		Pointer<OrthographicCamera> m_Camera;

		bool m_CanRotate;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_MoveSpeed;
		float m_RotationSpeed;

	private:
		void HandleEvent(const Event& event);
	
	private:
		static void __HandleEvent(const Event& event);
		static std::vector<OrthographicCameraController*> s_Cameras;
	};

}
#pragma once

#include "OrthographicCamera.h"
#include "../Events/Event.h"
#include "../Events/MouseEvent.h"
#include "../Events/WindowEvent.h"
#include <glm/glm.hpp>
#include <vector>

namespace ox {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
		friend class EventManager;

	public:
		OrthographicCameraController(float aspectRatio, bool canRotate = false, float moveSpeed = 5.0f);

		static Pointer<OrthographicCameraController> Create(float aspactRatio, bool canRotate = false, float moveSpeed = 5.0f);

		void Update(float deltaTime);
		const Pointer<OrthographicCamera>& GetCamera() const { return m_Camera; }
		OrthographicCameraBounds& GetBounds() { return m_Bounds; }
		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float zoom) { m_ZoomLevel = zoom; CalculateView(); }
		void Resize(float width, float height);

	private:
		float m_AspectRatio;
		float m_ZoomLevel;
		OrthographicCameraBounds m_Bounds;
		Pointer<OrthographicCamera> m_Camera;

		bool m_CanRotate;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };;
		float m_CameraRotation = 0.0f;
		float m_MoveSpeed;
		float m_RotationSpeed = 180.0f;

	private:
		void CalculateView();
		void HandleEvent(const Event& event);
	
	private:
		static void __HandleEvent(const Event& event);
		static std::vector<OrthographicCameraController*> s_Cameras;
	};

}
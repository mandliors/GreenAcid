#include "OrthographicCameraController.h"
#include "../Input/Input.h"
#include "../Base/KeyCodes.h"

namespace ox {

	std::vector<OrthographicCameraController*> OrthographicCameraController::s_Cameras;

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool canRotate, float moveSpeed)
		: m_AspectRatio(aspectRatio), m_ZoomLevel(1.0f), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(MakePointer<OrthographicCamera>(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top)), m_CanRotate(canRotate), m_MoveSpeed(moveSpeed)
	{
		s_Cameras.push_back(this);
	}

	Pointer<OrthographicCameraController> OrthographicCameraController::Create(float aspactRatio, bool canRotate, float moveSpeed)
	{
		return MakePointer<OrthographicCameraController>(aspactRatio, canRotate, moveSpeed);
	}

	void OrthographicCameraController::Update(float deltaTime)
	{
		if (Input::GetKey(OX_KEY_W))
			m_CameraPosition.y += m_MoveSpeed * deltaTime;
		else if (Input::GetKey(OX_KEY_S))
			m_CameraPosition.y -= m_MoveSpeed * deltaTime;
		if (Input::GetKey(OX_KEY_A))
			m_CameraPosition.x -= m_MoveSpeed * deltaTime;
		else if (Input::GetKey(OX_KEY_D))
			m_CameraPosition.x += m_MoveSpeed * deltaTime;

		if (m_CanRotate)
		{
			if (Input::GetKey(OX_KEY_Q))
				m_CameraRotation += m_RotationSpeed * deltaTime;
			else if (Input::GetKey(OX_KEY_E))
				m_CameraRotation -= m_RotationSpeed * deltaTime;

			m_Camera->SetRotation(m_CameraRotation);
		}
		m_Camera->SetPosition(m_CameraPosition);
		m_MoveSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::Resize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera->SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	void OrthographicCameraController::HandleEvent(const Event& event)
	{
		switch (event.GetEventType())
		{
			case EventType::MouseScrolled:
			{
				m_ZoomLevel -= static_cast<const MouseScrolledEvent&>(event).GetYOffset() * 0.25f;
				m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
				CalculateView();
				break;
			}
			case EventType::WindowResize:
			{
				const WindowResizeEvent& winEv = static_cast<const WindowResizeEvent&>(event);
				Resize((float)winEv.GetWidth(), (float)winEv.GetHeight());
				break;
			}
		}
	}

	void OrthographicCameraController::__HandleEvent(const Event& event)
	{
		for (auto* cam : s_Cameras)
			cam->HandleEvent(event);
	}

}
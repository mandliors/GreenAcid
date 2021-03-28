#include "OrthographicCameraController.h"
#include "Renderer2D.h"
#include "Input/Input.h"
#include "Base/KeyCodes.h"

namespace ox {

	std::vector<OrthographicCameraController*> OrthographicCameraController::s_Cameras;

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool canRotate, float moveSpeed, float rotationSpeed)
		: m_ZoomLevel(1.0f), m_Bounds({-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel}), m_Camera(MakePointer<OrthographicCamera>(m_Bounds.Left * m_ZoomLevel, m_Bounds.Right * m_ZoomLevel, m_Bounds.Bottom * m_ZoomLevel, m_Bounds.Top * m_ZoomLevel)), m_CanRotate(canRotate), m_MoveSpeed(moveSpeed), m_RotationSpeed(rotationSpeed)
	{
		Renderer2D::SetCoordinateSystemOrigin(CoordinateSystemOrigin::CENTER_SCREEN);
		s_Cameras.push_back(this);
	}
	OrthographicCameraController::OrthographicCameraController(float width, float height, bool canRotate, float moveSpeed, float rotationSpeed)
		: m_ZoomLevel(1.0f), m_Bounds({ -width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f }), m_Camera(MakePointer<OrthographicCamera>(m_Bounds.Left * m_ZoomLevel, m_Bounds.Right * m_ZoomLevel, m_Bounds.Bottom * m_ZoomLevel, m_Bounds.Top * m_ZoomLevel)), m_CanRotate(canRotate), m_MoveSpeed(moveSpeed), m_RotationSpeed(rotationSpeed)
	{
		Renderer2D::SetCoordinateSystemOrigin(CoordinateSystemOrigin::TOP_LEFT_CORNER);
		s_Cameras.push_back(this);
	}

	Pointer<OrthographicCameraController> OrthographicCameraController::Create(float aspactRatio, bool canRotate, float moveSpeed, float rotationSpeed)
	{
		return MakePointer<OrthographicCameraController>(aspactRatio, canRotate, moveSpeed, rotationSpeed);
	}
	Pointer<OrthographicCameraController> OrthographicCameraController::Create(float width, float height, bool canRotate, float moveSpeed, float rotationSpeed)
	{
		return MakePointer<OrthographicCameraController>(width, height, canRotate, moveSpeed, rotationSpeed);
	}

	void OrthographicCameraController::Update(float deltaTime)
	{
		if (Input::GetKey(OX_KEY_W))
			m_CameraPosition.y += m_MoveSpeed * m_ZoomLevel * deltaTime;
		else if (Input::GetKey(OX_KEY_S))
			m_CameraPosition.y -= m_MoveSpeed * m_ZoomLevel * deltaTime;
		if (Input::GetKey(OX_KEY_A))
			m_CameraPosition.x -= m_MoveSpeed * m_ZoomLevel * deltaTime;
		else if (Input::GetKey(OX_KEY_D))
			m_CameraPosition.x += m_MoveSpeed * m_ZoomLevel * deltaTime;

		if (m_CanRotate)
		{
			if (Input::GetKey(OX_KEY_Q))
				m_CameraRotation += m_RotationSpeed * deltaTime;
			else if (Input::GetKey(OX_KEY_E))
				m_CameraRotation -= m_RotationSpeed * deltaTime;

			m_Camera->SetRotation(m_CameraRotation);
		}
		m_Camera->SetPosition(m_CameraPosition);
	}

	void OrthographicCameraController::Resize(float aspectRatio)
	{
		m_Bounds = { -aspectRatio, aspectRatio, -1.0f, 1.0f };
		m_Camera->Resize(m_Bounds.GetWidth() / m_Bounds.GetHeight(), m_ZoomLevel);
	}

	void OrthographicCameraController::Resize(float width, float height)
	{
		m_Bounds = { -width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f };
		m_Camera->Resize(width, height, m_ZoomLevel);
	}

	void OrthographicCameraController::HandleEvent(const Event& event)
	{
		switch (event.GetEventType())
		{
			case EventType::MouseScrolled:
			{
				m_ZoomLevel -= static_cast<const MouseScrolledEvent&>(event).GetYOffset() * 0.25f;
				m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
				m_Camera->Resize(m_Bounds.GetWidth(), m_Bounds.GetHeight(), m_ZoomLevel);
				break;
			}
			case EventType::WindowResize:
			{
				const WindowResizeEvent& winEv = static_cast<const WindowResizeEvent&>(event);
				//Center screen
				if (Renderer2D::GetCoordinateSystemOrigin() == CoordinateSystemOrigin::CENTER_SCREEN)
					Resize((float)winEv.GetWidth() / (float)winEv.GetHeight());
				//Top left corner
				else
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
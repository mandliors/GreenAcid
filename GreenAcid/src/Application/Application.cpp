#include "Application.h"
#include "../Base/Base.h"

namespace GreenAcid {

	Pointer<GameWindow> Application::s_Window = nullptr;
	AppDescriptor Application::s_Descriptor;
	Pointer<OrthographicCameraController> Application::s_Camera = nullptr;
	Pointer<Framebuffer> Application::s_Framebuffer = nullptr;
	Pointer<Scene> Application::s_ActiveScene = nullptr;
	glm::vec2 Application::s_ViewportSize = { 0.0f, 0.0f };
	bool Application::s_ViewportFocused = false;
	bool Application::s_ViewportHovered = false;
	static bool s_AppInitialized = false;
	static bool s_Unminimised = false;

	static GameObject squareGameObject;

	void Application::Init(AppDescriptor& descriptor)
	{
		GA_ASSERT(!s_AppInitialized, "Application was already initialized");

		s_AppInitialized = true;
		
		s_Descriptor = descriptor;

		descriptor.Start = Application::OnStart;
		descriptor.Update = Application::OnUpdate;
		descriptor.Shutdown = Application::OnShutdown;

		s_Window = MakePointer<GameWindow>(descriptor);
		EventManager::DispatchEvent(EventType::WindowUnminimise, [](const Event& event)
		{
			s_Unminimised = true;
		});
	}

	void Application::Run()
	{
		s_Window->MainLoop();
	}

	bool Application::IsRunning()
	{
		return s_Window->IsOpen();
	}

	void Application::Close()
	{
		s_Window->Close();
	}

	float Application::GetFPS()
	{
		return s_Window->GetFPS();
	}

	glm::vec2 Application::GetPosition()
	{
		return s_Window->GetPosition();
	}

	uint32_t Application::GetWidth()
	{
		return s_Window->GetWidth();
	}

	uint32_t Application::GetHeight()
	{
		return s_Window->GetHeight();
	}

	Pointer<GameWindow> Application::GetOxygenWindow()
	{
		return s_Window;
	}

	void Application::OnStart()
	{
		s_Camera = OrthographicCameraController::Create((float)s_Descriptor.Width / (float)s_Descriptor.Height);
		
		FramebufferDescriptor desc;
		desc.Width = GameWindow::Get()->GetWidth();
		desc.Height = GameWindow::Get()->GetHeight();
		s_Framebuffer = Framebuffer::Create(desc);
		
		s_ActiveScene = MakePointer<Scene>();
		squareGameObject = s_ActiveScene->CreateGameObject("Square");
		squareGameObject.AddComponent<SpriteRenderer>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		if (s_Descriptor.Start) s_Descriptor.Start();
	}

	void Application::OnUpdate(float deltaTime)
	{
		if (s_ViewportFocused)
			s_Camera->Update(deltaTime);

		Renderer2D::ResetStats();
		s_Framebuffer->Bind();
		Renderer::Clear(0.1f, 0.1f, 0.1f, 1.0f);
		
		Renderer2D::BeginScene(s_Camera->GetCamera());
		s_ActiveScene->__Update(deltaTime);
		Renderer2D::EndScene();
		
		if (s_Descriptor.Update) s_Descriptor.Update(deltaTime);

		s_Framebuffer->Unbind();

		OnImGuiRender();
	}

	void Application::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		s_ViewportFocused = ImGui::IsWindowFocused();
		s_ViewportHovered = ImGui::IsWindowHovered();
		ImGuiLayer::BlockEvents(!s_ViewportFocused || !s_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if ((s_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) || s_Unminimised)
		{
			s_Unminimised = false;
			s_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			s_Framebuffer->Resize((uint32_t)s_ViewportSize.x, (uint32_t)s_ViewportSize.y);

			s_Camera->Resize(viewportPanelSize.x, viewportPanelSize.y);
		}
		ImGui::Image((void*)s_Framebuffer->GetColorAttachmentRendererID(), ImVec2{ s_ViewportSize.x, s_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		//Stats
		auto stats = Renderer2D::GetStats();
		ImGui::Begin("Statistics");
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
		ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());
		ImGui::Text("FPS: %f", GetFPS());
		
		if (squareGameObject)
		{
			ImGui::Separator();

			ImGui::Text("%s", squareGameObject.GetComponent<Tag>().TagString.c_str());
			ImGui::ColorPicker4("Square color", glm::value_ptr(squareGameObject.GetComponent<SpriteRenderer>().Color));

			ImGui::Separator();
		}

		ImGui::End();
	}

	void Application::OnShutdown()
	{
		if (s_Descriptor.Shutdown) s_Descriptor.Shutdown();
	}
}
#pragma once

#include "../../Vendor/entt/entt.hpp"
#include "../Components/Components.h"

namespace GreenAcid { class Application; }

namespace ox {
	
	class GameObject;

	class Scene
	{
		friend class GameObject;
		friend class GreenAcid::Application;

	public:
		Scene();
		~Scene();

		GameObject CreateGameObject(const std::string& name = std::string());

	private:
		void __Update(float deltaTime);

	private:
		entt::registry m_Registry;
		
	};
}
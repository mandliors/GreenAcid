#pragma once

#include "ECS/Scene/Scene.h"
#include "Vendor/EnTT/entt.hpp"

namespace ox {

	class GameObject
	{
	public:
		GameObject() = default;
		GameObject(entt::entity handle, Scene* scene)
			: m_EntityHandle(handle), m_Scene(scene) { }
		GameObject(const GameObject& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			OX_ASSERT(!HasComponent<T>(), "GameObject already has component");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			OX_ASSERT(HasComponent<T>(), "GameObject does not have component");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		bool RemoveComponent()
		{
			OX_ASSERT(HasComponent<T>(), "GameObject does not have component");
			return m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
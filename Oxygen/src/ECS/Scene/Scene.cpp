#include "Scene.h"
#include "Base/Defines.h"
#include "Renderer/Renderer2D.h"
#include "ECS/GameObject/GameObject.h"
#include <glm/glm.hpp>

namespace ox {


	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	GameObject Scene::CreateGameObject(const std::string& name)
	{
		GameObject gameObject = { m_Registry.create(), this };
		if (name.empty())
			gameObject.AddComponent<Tag>("GameObject");
		else
			gameObject.AddComponent<Tag>(name);
		gameObject.AddComponent<Transform>();
		return gameObject;
	}

	void Scene::__Update(float deltaTime)
	{
		auto group = m_Registry.group<Transform>(entt::get<SpriteRenderer>);
		for (auto entity : group)
		{
		#ifdef CPP_20_OR_MORE
			auto& [transform, spriteRenderer] = group.get<Transform, SpriteRenderer>(entity);
			Renderer2D::DrawQuad(transform, spriteRenderer.Color);
		#else
			auto components = group.get<Transform, SpriteRenderer>(entity);
			Renderer2D::DrawQuad(std::get<0>(components), std::get<1>(components).Color);
		#endif
		}
	}
}
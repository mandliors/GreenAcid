#pragma once

#include <glm/glm.hpp>

namespace ox {

	struct Tag
	{
		std::string TagString;

		Tag() = default;
		Tag(const Tag&) = default;
		Tag(const std::string& tag)
			: TagString(tag) { }

		operator std::string& () { return TagString; }
		operator const std::string& () const { return TagString; }
	};

	struct Transform
	{
		glm::mat4 TransformMatrix{ 1.0f };

		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::mat4& transform)
			: TransformMatrix(transform) { }

		operator glm::mat4& () { return TransformMatrix; }
		operator const glm::mat4& () const { return TransformMatrix; }
	};

	struct SpriteRenderer
	{
		glm::vec4 Color{ 1.0, 1.0f, 1.0f, 1.0f };

		SpriteRenderer() = default;
		SpriteRenderer(const SpriteRenderer&) = default;
		SpriteRenderer(const glm::vec4 color)
			: Color(color) { }
	};
}
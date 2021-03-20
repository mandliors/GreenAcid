#pragma once

#include "../Base/Base.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <unordered_map>

namespace ox {

	class Shader
	{
	public:
		Shader(const std::string& path);
		Shader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		static Pointer<Shader> Create(const std::string& path);
		static Pointer<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);

		void Bind();
		void Unbind();

		const std::string& GetName() const { return m_Name; }

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformInt2(const std::string& name, const glm::ivec2& value);
		void UploadUniformInt3(const std::string& name, const glm::ivec4& value);
		void UploadUniformInt4(const std::string& name, const glm::ivec4& value);

		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& value);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
		mutable std::unordered_map<std::string, GLint> m_UniformLocations;

	private:
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		GLint GetUniformLocation(const std::string& name) const;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Pointer<Shader>& shader);
		void Add(const Pointer<Shader>& shader);
		Pointer<Shader> Load(const std::string& path);
		Pointer<Shader> Load(const std::string& name, const std::string& path);

		Pointer<Shader> Get(const std::string& name) const;

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Pointer<Shader>> m_Shaders;
	};
}
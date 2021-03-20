#include "Shader.h"
#include "../Debug/Debug.h"
#include "../Base/Base.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <fstream>

namespace ox {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		OX_ASSERT(false, "Unknown shader type");
		return 0;
	}

	Shader::Shader(const std::string& path)
	{
		std::string source = ReadFile(path);
		std::unordered_map<GLenum, std::string> shaderSources = PreProcess(source);
		Compile(shaderSources);

		//Get name from path
		size_t lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		size_t lastDot = path.rfind('.');
		size_t count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_Name = path.substr(lastSlash, count);
	}

	Shader::Shader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	Pointer<Shader> Shader::Create(const std::string& path)
	{
		return std::make_shared<Shader>(path);
	}

	Pointer<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		return std::make_shared<Shader>(name, vertexSource, fragmentSource);
	}

	void Shader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind()
	{
		glUseProgram(0);
	}

	void Shader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniform1i(location, value);
	}

	void Shader::UploadUniformInt2(const std::string& name, const glm::ivec2& value)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniform2i(location, value.x, value.y);
	}

	void Shader::UploadUniformInt3(const std::string& name, const glm::ivec4& value)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniform3i(location, value.x, value.y, value.z);
	}

	void Shader::UploadUniformInt4(const std::string& name, const glm::ivec4& value)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniform4i(location, value.x, value.y, value.z, value.w);
	}

	void Shader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniform1iv(location, count, values);
	}

	void Shader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniform1f(location, value);
	}

	void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniform2f(location, value.x, value.y);
	}

	void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& vector)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = GetUniformLocation(name);
		OX_ASSERT(location != -1, "Uniform '" + name + "' does not exist");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		OX_ASSERT(shaderSources.size() <= 2, "Only two shaders are supported");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& item : shaderSources)
		{
			GLenum type = item.first;
			const std::string& source = item.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);

			GLint isComplied = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isComplied);
			if (isComplied == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				Debug::Error(infoLog.data());
				OX_ASSERT(false, "Shader compilation failure");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (auto& id : glShaderIDs)
				glDeleteShader(id);

			Debug::Error(infoLog.data());
			OX_ASSERT(false, "Shader linking failure");
			return;
		}

		for (auto& id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;
	}

	std::string Shader::ReadFile(const std::string& path)
	{
		std::ifstream in(path, std::ios::in | std::ios::binary);
		OX_ASSERT(in, "Could not open file " + path);

		std::string result;
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();

		return result;
	}

	std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	GLint Shader::GetUniformLocation(const std::string& name) const
	{
		if (m_UniformLocations.find(name) != m_UniformLocations.end())
			return m_UniformLocations[name];

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocations[name] = location;
		return location;
	}

	void ShaderLibrary::Add(const std::string& name, const Pointer<Shader>& shader)
	{
		OX_ASSERT(!Exists(name), "Shader " + name + " already exists");
		m_Shaders[name] = shader;
	}
	
	void ShaderLibrary::Add(const Pointer<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	Pointer<Shader> ShaderLibrary::Load(const std::string& path)
	{
		Pointer<Shader> shader = std::make_shared<Shader>(path);
		Add(shader);
		return shader;
	}

	Pointer<Shader> ShaderLibrary::Load(const std::string& name, const std::string& path)
	{
		Pointer<Shader> shader = std::make_shared<Shader>(path);
		Add(name, shader);
		return shader;
	}

	Pointer<Shader> ShaderLibrary::Get(const std::string& name) const
	{
		OX_ASSERT(Exists(name), "Shader " + name + " not found");
		return m_Shaders.at(name);
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
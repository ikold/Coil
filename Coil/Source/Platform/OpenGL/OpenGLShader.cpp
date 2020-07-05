#include "pch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include  <glm/gtc/type_ptr.hpp>


namespace Coil
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		CL_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const RString<>& filePath)
	{
		CL_PROFILE_FUNCTION()

		const RString<> source   = File::Load(filePath);
		const auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// TODO Implement this for RString
		const std::string filepath = filePath->CString();

		// Extract name from filepath
		auto lastSlash     = filepath.find_last_of("/\\");
		lastSlash          = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		const auto lastDot = filepath.rfind('.');
		const auto count   = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		Name               = filepath.substr(lastSlash, count).c_str();
	}

	OpenGLShader::OpenGLShader(RString<> name, const RString<>& vertexSource, const RString<>& fragmentSource)
		: Name(Move(name))
	{
		CL_PROFILE_FUNCTION()

		std::unordered_map<GLenum, RString<>> sources;
		sources[GL_VERTEX_SHADER]   = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		CL_PROFILE_FUNCTION()

		glDeleteProgram(RendererID);
	}

	std::unordered_map<uint32, RString<>> OpenGLShader::PreProcess(const RString<>& source)
	{
		CL_PROFILE_FUNCTION()

		std::unordered_map<uint32, RString<>> shaderSources;

		// TODO Implement this for RString
		const std::string sourceString = source->CString();

		const char* typeToken        = "#type";
		const size_t typeTokenLength = strlen(typeToken);
		size_t pos                   = sourceString.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			const size_t eol = sourceString.find_first_of("\r\n", pos);
			CL_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			const size_t begin = pos + typeTokenLength + 1;
			std::string type   = sourceString.substr(begin, eol - begin);
			CL_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			const size_t nextLinePos                  = sourceString.find_first_not_of("\r\n", eol);
			pos                                       = sourceString.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = sourceString.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? sourceString.size() - 1 : nextLinePos)).c_str();
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<uint32, RString<>>& shaderSources)
	{
		CL_PROFILE_FUNCTION()

		const GLuint program = glCreateProgram();
		CL_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs{};

		int glShaderIDIndex = 0;
		for (const auto& kv : shaderSources)
		{
			const GLenum type       = kv.first;
			const RString<>& source = kv.second;

			const GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source->CString();
			glShaderSource(shader, 1, &sourceCStr, nullptr);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				CL_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, static_cast<int32*>(&isLinked));
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			CL_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(program, id);
	}

	void OpenGLShader::Bind() const
	{
		CL_PROFILE_FUNCTION_LOW()

		glUseProgram(RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		CL_PROFILE_FUNCTION_LOW()

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const RString<>& name, int32 value) const
	{
		CL_PROFILE_FUNCTION_LOW()

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat(const RString<>& name, float32 value) const
	{
		CL_PROFILE_FUNCTION_LOW()

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const RString<>& name, const glm::vec2& vector) const
	{
		CL_PROFILE_FUNCTION_LOW()

		UploadUniformFloat2(name, vector);
	}

	void OpenGLShader::SetFloat3(const RString<>& name, const glm::vec3& vector) const
	{
		CL_PROFILE_FUNCTION_LOW()

		UploadUniformFloat3(name, vector);
	}

	void OpenGLShader::SetFloat4(const RString<>& name, const glm::vec4& vector) const
	{
		CL_PROFILE_FUNCTION_LOW()

		UploadUniformFloat4(name, vector);
	}

	void OpenGLShader::SetMat3(const RString<>& name, const glm::mat3& matrix) const
	{
		CL_PROFILE_FUNCTION_LOW()

		UploadUniformMat3(name, matrix);
	}

	void OpenGLShader::SetMat4(const RString<>& name, const glm::mat4& matrix) const
	{
		CL_PROFILE_FUNCTION_LOW()

		UploadUniformMat4(name, matrix);
	}


	void OpenGLShader::UploadUniformInt(const RString<>& name, int32 value) const
	{
		const GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const RString<>& name, float32 value) const
	{
		const GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const RString<>& name, const glm::vec2& vector) const
	{
		const GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniform2f(location, vector.x, vector.y);
	}

	void OpenGLShader::UploadUniformFloat3(const RString<>& name, const glm::vec3& vector) const
	{
		const GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void OpenGLShader::UploadUniformFloat4(const RString<>& name, const glm::vec4& vector) const
	{
		const GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::UploadUniformMat3(const RString<>& name, const glm::mat3& matrix) const
	{
		const GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const RString<>& name, const glm::mat4& matrix) const
	{
		const GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
	}
}

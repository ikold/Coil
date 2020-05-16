#include "pch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include  <glm/gtc/type_ptr.hpp>


namespace Coil
{
	OpenGLShader::OpenGLShader(const RString<String>& vertexSource, const RString<String>& fragmentSource)
	{
		// Create an empty vertex shader handle
		const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		const GLchar* source = vertexSource->CString();
		glShaderSource(vertexShader, 1, &source, nullptr);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			Logger::Error(infoLog.data());
			CL_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		source = fragmentSource->CString();
		glShaderSource(fragmentShader, 1, &source, nullptr);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			Logger::Error(infoLog.data());
			CL_CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(RendererID, vertexShader);
		glAttachShader(RendererID, fragmentShader);

		// Link our program
		glLinkProgram(RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(RendererID, GL_LINK_STATUS, static_cast<int*>(&isLinked));
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			Logger::Error(infoLog.data());
			CL_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(RendererID, vertexShader);
		glDetachShader(RendererID, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	
	void OpenGLShader::UploadUniformInt(const RString<String>& name, int32 value) const
	{
		GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const RString<String>& name, float32 value) const
	{
		GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const RString<String>& name, const glm::vec2& vector) const
	{
		GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniform2f(location, vector.x, vector.y);
	}

	void OpenGLShader::UploadUniformFloat3(const RString<String>& name, const glm::vec3& vector) const
	{
		GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void OpenGLShader::UploadUniformFloat4(const RString<String>& name, const glm::vec4& vector) const
	{
		GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void OpenGLShader::UploadUniformMat3(const RString<String>& name, const glm::mat3& matrix) const
	{
		GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const RString<String>& name, const glm::mat4& matrix) const
	{
		GLint location = glGetUniformLocation(RendererID, name->CString());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}

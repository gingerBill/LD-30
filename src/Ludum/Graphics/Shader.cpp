#include <Ludum/Graphics/Shader.hpp>

#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

namespace gb
{
	Shader::Shader(const std::string& shaderCode, GLenum shaderType)
		:m_object(0)
		,m_refCount(nullptr)
	{
		m_object = glCreateShader(shaderType);
		if (m_object == 0)
			throw std::runtime_error("glCreateShader failed");

		const char* code = shaderCode.c_str();
		glShaderSource(m_object, 1, (const GLchar**)&code, nullptr);

		//compile
		glCompileShader(m_object);

		GLint status;
		glGetShaderiv(m_object, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			std::string msg("Compile failure in shader:\n");

			GLint infoLogLength;
			glGetShaderiv(m_object, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* strInfoLog = new char[infoLogLength + 1];
			glGetShaderInfoLog(m_object, infoLogLength, nullptr, strInfoLog);
			msg += strInfoLog;
			delete[] strInfoLog;

			glDeleteShader(m_object);
			m_object = 0;
			throw std::runtime_error(msg);
		}

		m_refCount = new unsigned int;
		*m_refCount = 1;
	}

	Shader::Shader(const Shader& other)
		:m_object(other.m_object)
	 	,m_refCount(other.m_refCount)
	{
		retain();
	}

	Shader::~Shader()
	{
		if (m_refCount)
			release();
	}

	GLuint Shader::object() const
	{
		return m_object;
	}

	Shader& Shader::operator=(const Shader& other)
	{
		release();

		m_object = other.m_object;
		m_refCount = other.m_refCount;

		retain();

		return *this;
	}

	Shader Shader::loadFromFile(const std::string& filePath, GLenum shaderType)
	{
		std::ifstream f;
		f.open(filePath.c_str(), std::ios::in | std::ios::binary);

		if (!f.is_open())
		{
			throw std::runtime_error(std::string("Failed to open file: ") + filePath);
		}

		std::stringstream buffer;
		buffer << f.rdbuf();

		Shader shader(buffer.str(), shaderType);
		return shader;
	}

	void Shader::retain()
	{
		assert(m_refCount);
		*m_refCount += 1;
	}

	void Shader::release()
	{
		assert(m_refCount && *m_refCount > 0);
		*m_refCount -= 1;
		if (*m_refCount == 0)
		{
			glDeleteShader(m_object);
			m_object = 0;
			delete m_refCount;
			m_refCount = nullptr;
		}
	}
}

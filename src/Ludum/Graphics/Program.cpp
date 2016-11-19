#define GLEW_STATIC
#include <GL/glew.h>

#include <Ludum/Graphics/Program.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>
#include <iostream>
#include <fstream>

namespace gb
{
	Program* Program::loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
	{
		std::vector<Shader> shaders;
		shaders.push_back(Shader::loadFromFile(vertexShaderFilename, GL_VERTEX_SHADER));
		shaders.push_back(Shader::loadFromFile(fragmentShaderFilename, GL_FRAGMENT_SHADER));
		return new Program(shaders);
	}

	Program* Program::loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader)
	{
		std::vector<Shader> shaders;
		shaders.push_back(Shader(vertexShader, GL_VERTEX_SHADER));
		shaders.push_back(Shader(fragmentShader, GL_FRAGMENT_SHADER));
		return new Program(shaders);
	}


	Program::Program(const std::vector<Shader>& shaders)
	    :m_object(0)
	{
		if (shaders.size() <= 0)
			throw std::runtime_error("No shaders were provided to create the program\n");

		//create the program object
		m_object = glCreateProgram();
		if (m_object == 0)
			throw std::runtime_error("glCreateProgram failed\n");

		//attach all the shaders
		for (unsigned int i = 0; i < shaders.size(); ++i)
			glAttachShader(m_object, shaders[i].object());

		//link the shaders together
		glLinkProgram(m_object);

		//detach all the shaders
		for (unsigned int i = 0; i < shaders.size(); ++i)
			glDetachShader(m_object, shaders[i].object());

		//throw exception if linking failed
		GLint status;
		glGetProgramiv(m_object, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			std::string msg("Program linking failure: ");

			GLint infoLogLength;
			glGetProgramiv(m_object, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* strInfoLog = new char[infoLogLength + 1];
			glGetProgramInfoLog(m_object, infoLogLength, NULL, strInfoLog);
			msg += strInfoLog;
			delete[] strInfoLog;

			glDeleteProgram(m_object);
			m_object = 0;
			msg += '\n';
			throw std::runtime_error(msg);
		}
	}

	Program::~Program()
	{
		// Might be 0 if ctor fails by throwing exception
		if (m_object != 0)
			glDeleteProgram(m_object);
	}

	GLuint Program::object() const
	{
		return m_object;
	}

	void Program::use() const
	{
		glUseProgram(m_object);
	}

	bool Program::isInUse() const
	{
		GLint currentProgram = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		return (currentProgram == (GLint)m_object);
	}

	void Program::stopUsing() const
	{
		assert(isInUse());
		glUseProgram(0);
	}

	GLint Program::attrib(const GLchar* attribName) const
	{
		if (!attribName)
			throw std::runtime_error("attribName was nullptr");

		GLint attrib = glGetAttribLocation(m_object, attribName);
		if (attrib == -1)
			throw std::runtime_error(std::string("Program attribute not found: ") + attribName);

		return attrib;
	}

	GLint Program::uniform(const GLchar* uniformName) const
	{
		if (!uniformName)
			throw std::runtime_error("uniformName was nullptr");

		GLint uniform = glGetUniformLocation(m_object, uniformName);
		if (uniform == -1)
			throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);

		return uniform;
	}

#define ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX)                                 \
                                                                                                     \
	void Program::setAttrib(const GLchar* name, OGL_TYPE v0)                                         \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glVertexAttrib##TYPE_PREFIX##1##TYPE_SUFFIX(attrib(name), v0);                               \
	}                                                                                                \
	void Program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1)                            \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glVertexAttrib##TYPE_PREFIX##2##TYPE_SUFFIX(attrib(name), v0, v1);                           \
	}                                                                                                \
	void Program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2)               \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glVertexAttrib##TYPE_PREFIX##3##TYPE_SUFFIX(attrib(name), v0, v1, v2);                       \
	}                                                                                                \
	void Program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3)  \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glVertexAttrib##TYPE_PREFIX##4##TYPE_SUFFIX(attrib(name), v0, v1, v2, v3);                   \
	}                                                                                                \
                                                                                                     \
	void Program::setAttrib1v(const GLchar* name, const OGL_TYPE* v)                                 \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glVertexAttrib##TYPE_PREFIX##1##TYPE_SUFFIX##v(attrib(name), v);                             \
	}                                                                                                \
	void Program::setAttrib2v(const GLchar* name, const OGL_TYPE* v)                                 \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glVertexAttrib##TYPE_PREFIX##2##TYPE_SUFFIX##v(attrib(name), v);                             \
	}                                                                                                \
	void Program::setAttrib3v(const GLchar* name, const OGL_TYPE* v)                                 \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glVertexAttrib##TYPE_PREFIX##3##TYPE_SUFFIX##v(attrib(name), v);                             \
	}                                                                                                \
	void Program::setAttrib4v(const GLchar* name, const OGL_TYPE* v)                                 \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glVertexAttrib##TYPE_PREFIX##4##TYPE_SUFFIX##v(attrib(name), v);                             \
	}                                                                                                \
                                                                                                     \
	void Program::setUniform(const GLchar* name, OGL_TYPE v0)                                        \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glUniform1##TYPE_SUFFIX(uniform(name), v0);                                                  \
	}                                                                                                \
	void Program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1)                           \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glUniform2##TYPE_SUFFIX(uniform(name), v0, v1);                                              \
	}                                                                                                \
	void Program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2)              \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glUniform3##TYPE_SUFFIX(uniform(name), v0, v1, v2);                                          \
	}                                                                                                \
	void Program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glUniform4##TYPE_SUFFIX(uniform(name), v0, v1, v2, v3);                                      \
	}                                                                                                \
                                                                                                     \
	void Program::setUniform1v(const GLchar* name, const OGL_TYPE* v, GLsizei count)                 \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glUniform1##TYPE_SUFFIX##v(uniform(name), count, v);                                         \
	}                                                                                                \
	void Program::setUniform2v(const GLchar* name, const OGL_TYPE* v, GLsizei count)                 \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glUniform2##TYPE_SUFFIX##v(uniform(name), count, v);                                         \
	}                                                                                                \
	void Program::setUniform3v(const GLchar* name, const OGL_TYPE* v, GLsizei count)                 \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glUniform3##TYPE_SUFFIX##v(uniform(name), count, v);                                         \
	}                                                                                                \
	void Program::setUniform4v(const GLchar* name, const OGL_TYPE* v, GLsizei count)                 \
	{                                                                                                \
		assert(isInUse());                                                                           \
		glUniform4##TYPE_SUFFIX##v(uniform(name), count, v);                                         \
	}

	ATTRIB_N_UNIFORM_SETTERS(GLfloat, , f);
	ATTRIB_N_UNIFORM_SETTERS(GLdouble, , d);
	ATTRIB_N_UNIFORM_SETTERS(GLint, I, i);
	ATTRIB_N_UNIFORM_SETTERS(GLuint, I, ui);

	void Program::setUniformMatrix2(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose)
	{
		assert(isInUse());
		glUniformMatrix2fv(uniform(name), count, transpose, v);
	}

	void Program::setUniformMatrix3(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose)
	{
		assert(isInUse());
		glUniformMatrix3fv(uniform(name), count, transpose, v);
	}

	void Program::setUniformMatrix4(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose)
	{
		assert(isInUse());
		glUniformMatrix4fv(uniform(name), count, transpose, v);
	}

	void Program::setUniform(const GLchar* name, const glm::mat2& m, GLboolean transpose)
	{
		assert(isInUse());
		glUniformMatrix2fv(uniform(name), 1, transpose, glm::value_ptr(m));
	}

	void Program::setUniform(const GLchar* name, const glm::mat3& m, GLboolean transpose)
	{
		assert(isInUse());
		glUniformMatrix3fv(uniform(name), 1, transpose, glm::value_ptr(m));
	}

	void Program::setUniform(const GLchar* name, const glm::mat4& m, GLboolean transpose)
	{
		assert(isInUse());
		glUniformMatrix4fv(uniform(name), 1, transpose, glm::value_ptr(m));
	}

	void Program::setUniform(const GLchar* uniformName, const glm::vec3& v)
	{
		setUniform3v(uniformName, glm::value_ptr(v));
	}

	void Program::setUniform(const GLchar* uniformName, const glm::vec4& v)
	{
		setUniform4v(uniformName, glm::value_ptr(v));
	}

	void Program::setUniform(const GLchar* uniformName, const sf::Vector2f& v)
	{
		setUniform2v(uniformName, glm::value_ptr(glm::vec2(v.x, v.y)));
	}

	void Program::setUniform(const GLchar* uniformName, const sf::Vector3f& v)
	{
		setUniform3v(uniformName, glm::value_ptr(glm::vec3(v.x, v.y, v.z)));
	}

	void Program::setUniform(const GLchar* uniformName, const sf::Color& c)
	{
		glm::vec4 color;
		color.r = (float)c.r/255.0f;
		color.g = (float)c.g/255.0f;
		color.b = (float)c.b/255.0f;
		color.a = (float)c.a/255.0f;
		setUniform4v(uniformName, glm::value_ptr(color));
	}
}

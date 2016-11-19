#ifndef Ludum_Graphics_Program_hpp
#define Ludum_Graphics_Program_hpp

#include <Ludum/Graphics/Shader.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

#include <glm/glm.hpp>

#include <vector>

namespace gb
{
	class Program
	{
	public:
		explicit Program(const std::vector<Shader>& shaders);
		~Program();

		static Program* loadFromFile(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
		static Program* loadFromMemory(const std::string& vertexShader, const std::string& fragmentShader);

		GLuint object() const;

		void use() const;
		bool isInUse() const;
		void stopUsing() const;

		GLint attrib(const GLchar* attribName) const;
		GLint uniform(const GLchar* uniformName) const;

	#define GB_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE)                                               \
		void setAttrib(const GLchar* attribName, OGL_TYPE v0);                                          \
		void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1);                             \
		void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2);                \
		void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3);   \
	                                                                                                    \
		void setAttrib1v(const GLchar* attribName, const OGL_TYPE* v);                                  \
		void setAttrib2v(const GLchar* attribName, const OGL_TYPE* v);                                  \
		void setAttrib3v(const GLchar* attribName, const OGL_TYPE* v);                                  \
		void setAttrib4v(const GLchar* attribName, const OGL_TYPE* v);                                  \
	                                                                                                    \
		void setUniform(const GLchar* uniformName, OGL_TYPE v0);                                        \
		void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1);                           \
		void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2);              \
		void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
	                                                                                                    \
		void setUniform1v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1);             \
		void setUniform2v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1);             \
		void setUniform3v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1);             \
		void setUniform4v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count = 1);

		GB_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLfloat)
		GB_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLdouble)
		GB_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLint)
		GB_PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLuint)

		void setUniformMatrix2(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
		void setUniformMatrix3(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
		void setUniformMatrix4(const GLchar* uniformName, const GLfloat* v, GLsizei count=1, GLboolean transpose=GL_FALSE);
		void setUniform(const GLchar* uniformName, const glm::mat2& m, GLboolean transpose=GL_FALSE);
		void setUniform(const GLchar* uniformName, const glm::mat3& m, GLboolean transpose=GL_FALSE);
		void setUniform(const GLchar* uniformName, const glm::mat4& m, GLboolean transpose=GL_FALSE);
		void setUniform(const GLchar* uniformName, const glm::vec3& v);
		void setUniform(const GLchar* uniformName, const glm::vec4& v);
		void setUniform(const GLchar* uniformName, const sf::Vector2f& v);
		void setUniform(const GLchar* uniformName, const sf::Vector3f& v);
		void setUniform(const GLchar* uniformName, const sf::Color& c);


	private:
		GLuint m_object;

		Program(const Program&) = delete;
		const Program& operator=(const Program&) = delete;
	};
}

#endif

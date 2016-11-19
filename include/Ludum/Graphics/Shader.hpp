#ifndef Ludum_Graphics_Shader_hpp
#define Ludum_Graphics_Shader_hpp

#include <SFML/OpenGL.hpp>

#include <string>

namespace gb
{
	class Shader
	{
	public:
		Shader(const std::string& shaderCode, GLenum shaderType);
		Shader(const Shader& other);
		Shader& operator =(const Shader& other);
		~Shader();

		static Shader loadFromFile(const std::string& filePath, GLenum shaderType);

		GLuint object() const;

	private:
		GLuint m_object;
		unsigned int* m_refCount;

		void retain();
		void release();

	};
}

#endif

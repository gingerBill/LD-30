#ifndef Ludum_Graphics_Texture_hpp
#define Ludum_Graphics_Texture_hpp

#include <SFML/Graphics/Image.hpp>
#include <SFMl/OpenGL.hpp>

namespace gb
{
	class Texture
	{
	public:
		Texture();
		Texture(sf::Image image,
		        GLint minMagFilter = GL_NEAREST,
		        GLint wrapMode = GL_CLAMP_TO_EDGE);
		~Texture();

		const sf::Image& getImage() const;
		GLuint object() const;
		GLfloat originalWidth() const;
		GLfloat originalHeight() const;

		// static Texture* loadFromFile(const std::string& filename,
		//                              GLint minMagFilter = GL_NEAREST,
		//                              GLint wrapMode = GL_CLAMP_TO_EDGE);

		void loadFromFile(const std::string& filename,
		                  GLint minMagFilter = GL_NEAREST,
		                  GLint wrapMode = GL_CLAMP_TO_EDGE);

	private:
		GLuint m_object;
		GLfloat m_originalWidth;
		GLfloat m_originalHeight;
		sf::Image m_image;

		Texture(const Texture&) = delete;
		const Texture& operator=(const Texture&) = delete;
	};
}

#endif

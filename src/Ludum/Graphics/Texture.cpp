#include <Ludum/Graphics/Texture.hpp>

#include <stdexcept>

namespace gb
{
	Texture::Texture()
	{
	}

	Texture::Texture(sf::Image image, GLint minMagFiler, GLint wrapMode)
		:m_originalWidth( (GLfloat)image.getSize().x)
		,m_originalHeight((GLfloat)image.getSize().y)
	{
		m_image = image;
		// make the `correct` way up
		//image.flipVertically();

		glGenTextures(1, &m_object);
		glBindTexture(GL_TEXTURE_2D, m_object);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFiler);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFiler);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexImage2D(GL_TEXTURE_2D,
		             0,
		             GL_RGBA,
		             image.getSize().x,
		             image.getSize().y,
		             0,
		             GL_RGBA,
		             GL_UNSIGNED_BYTE,
		             image.getPixelsPtr());
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_object);
	}

	const sf::Image& Texture::getImage() const
	{
		return m_image;
	}

	void Texture::loadFromFile(const std::string& filename, GLint minMagFiler, GLint wrapMode)
	{
		sf::Image image;
		image.loadFromFile(filename);

		Texture(image, minMagFiler, wrapMode);
	}

	GLuint Texture::object() const
	{
		return m_object;
	}

	GLfloat Texture::originalWidth() const
	{
		return m_originalWidth;
	}

	GLfloat Texture::originalHeight() const
	{
		return m_originalHeight;
	}
}

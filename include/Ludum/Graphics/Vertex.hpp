#ifndef Ludum_Graphics_Vertex_hpp
#define Ludum_Graphics_Vertex_hpp

#include <SFML/Graphics/Color.hpp>
#include <glm/glm.hpp>

namespace gb
{
	class Vertex
	{
	public:
		Vertex()
			:m_position(0, 0, 0)
			,m_texCoord(0, 0)
		{
		}

		Vertex(const glm::vec3& position,
		       const glm::vec2& texCoord)
			:m_position(position)
			,m_texCoord(texCoord)
		{
		}
		Vertex(float x, float y, float z, float u, float v)
			:m_position(x, y, z)
			,m_texCoord(u, v)
		{
		}


		glm::vec3 getPosition() const { return m_position; }
		glm::vec2 getTexCoord() const { return m_texCoord; }


		Vertex& setPosition(const glm::vec3& position)
		{
			m_position = position;
			return *this;
		}

		Vertex& setPosition(float x, float y, float z)
		{
			m_position = glm::vec3(x, y, z);
			return *this;
		}

		Vertex& setTexCoord(const glm::vec2& texCoord)
		{
			m_texCoord = texCoord;
			return *this;
		}

		Vertex& setTexCoord(float u, float v)
		{
			m_texCoord = glm::vec2(u, v);
			return *this;
		}


		static const int SIZE = 5;

	private:
		glm::vec3 m_position;
		glm::vec2 m_texCoord;
	};
}

#endif

#include <Ludum/Level/Sprite.hpp>

#include <Ludum/Common.hpp>
#include <Ludum/Level/Level.hpp>

namespace gb
{
		Sprite::Sprite()
			:Transformable()
			,m_mesh()
			,m_faceCamera(false)
		{
		}

		Sprite::Sprite(int tileType)
			:Sprite()
		{
			generateMesh(tileType);
		}

		void Sprite::setTextureCoords(int tileType)
		{
			generateMesh(tileType);
		}

		void Sprite::update(sf::Time dt)
		{

		}

		void Sprite::draw(Program* shaderProgram)
		{
			shaderProgram->setUniform("transform", getTransform());
			m_mesh.draw(shaderProgram);
		}

		void Sprite::faceCamera(glm::vec3 cameraPos)
		{
			if (!m_faceCamera)
				return;

			glm::vec3 v = cameraPos - getPosition();
			float a = -glm::degrees(glm::atan(v.z/v.x));

			if (v.x < 0)
				a += 180;

			a += 90;

			setRotation(a, 0, 1, 0);
		}

		void Sprite::setFaceCamera(bool t)
		{
			m_faceCamera = t;
		}

		void Sprite::generateMesh(int tileType)
		{
			glm::vec4 uv = getTextureCoords(tileType);

			glm::vec3 size = Level::getTileSize();

			std::vector<Vertex> vertices;
			std::vector<int> indices;

			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(0);

			indices.push_back(0);
			indices.push_back(3);
			indices.push_back(2);
			indices.push_back(2);
			indices.push_back(1);
			indices.push_back(0);

			vertices.push_back(Vertex(0, 0, 0, uv[0], uv[1]));
			vertices.push_back(Vertex(size.x, 0, 0, uv[2], uv[1]));
			vertices.push_back(Vertex(size.x, size.y, 0, uv[2], uv[3]));
			vertices.push_back(Vertex(0, size.y, 0, uv[0], uv[3]));


			m_mesh.addVertices(vertices, indices);

			setOrigin(0.5, 0, 0);
		}
}

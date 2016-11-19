#ifndef Ludum_Level_Sprite_hpp
#define Ludum_Level_Sprite_hpp

#include <Ludum/Graphics/Transformable.hpp>
#include <Ludum/Graphics/Program.hpp>
#include <Ludum/Graphics/Mesh.hpp>

#include <SFML/System/Time.hpp>

namespace gb
{
	class Sprite : public Transformable
	{
	public:
		Sprite();
		Sprite(int tileType);

		virtual void update(sf::Time dt);

		void setTextureCoords(int tileType);

		void draw(Program* shaderProgram);

		void faceCamera(glm::vec3 cameraPos);

		void setFaceCamera(bool t);

	protected:
		void generateMesh(int tileType);

		Mesh m_mesh;
		bool m_faceCamera;

	};
}

#endif

#ifndef Ludum_Level_Level_hpp
#define Ludum_Level_Level_hpp

#include <Ludum/Graphics/Mesh.hpp>
#include <Ludum/Level/Player.hpp>
#include <Ludum/Level/Sprite.hpp>
#include <Ludum/Level/Door.hpp>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

#include <string>
#include <memory>

namespace gb
{
	class Level
	{
	public:
		enum WorldType
		{
			OverWorld,
			UnderWorld,
		};

		Level();

		~Level();

		bool loadFromFile(const std::string& over, const std::string& under);

		const Mesh& getMesh() const;
		const WorldType& getCurrentWorldType() const;

		void update(sf::Time dt);
		void draw(gb::Program* shaderProgram);

		Camera* getPlayerCamera();

		glm::vec3 checkCollision(const sf::FloatRect& collisionBox) const;
		glm::vec3 collisionAdjustment(sf::FloatRect o1, sf::FloatRect o2) const;

		static glm::vec3 getTileSize();

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		Level& setPixel(unsigned int x, unsigned int z, WorldType w, const sf::Color& color);
		sf::Color getPixel(unsigned int x, unsigned int z, WorldType w) const;

		bool levelCompleted() const;

		unsigned int getNumberOfPortalsLeft() const;
		unsigned int getNumberOfKeys() const;

	private:
		void generateLevel();

		void generateWorld(std::vector<Vertex>& vertices, std::vector<int>& indices, WorldType w);

		enum class FaceNormal
		{
			X,
			Y,
			Z,
		};
		void addFace(std::vector<int>& indices, size_t startLocation, bool direction);
		void addVertices(std::vector<Vertex>& vertices, float i, float j, float k, FaceNormal n, int tileType);

		void addSpecial(int blueValue, float i, float j, WorldType w);
		void addFlora(float i, float j, WorldType w);
		bool addPortal(float i, float j, WorldType w);

		void updatePortal(sf::Time dt);

		int m_seed;

		sf::Image m_overWorld;
		sf::Image m_underWorld;
		WorldType m_currentWorld;

		glm::vec3 m_spawn;

		Player m_player;

		Mesh m_mesh;

		std::vector<Sprite> m_sprites;
		std::vector<Sprite> m_portals;

		std::vector<Sprite> m_potions;
		std::vector<Sprite> m_keys;

		std::vector<Door>   m_doors;


		Sprite m_flag;

		sf::Time m_portalTravelCooldown;
		sf::Time m_portalPlaceCooldown;
		bool m_portalFlip;

		sf::Time m_totalTime;

		bool m_levelCompleted;

		int m_keysLeft;
		int m_portalsLeft;
	};
}

#endif

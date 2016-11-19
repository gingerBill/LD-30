#include <Ludum/Level/Level.hpp>

#include <Ludum/Common.hpp>
#include <Ludum/Sound.hpp>

#include <SFML/Window/Keyboard.hpp>

#include <cmath>

namespace gb
{
	static const float TILE_WIDTH  = 1;
	static const float TILE_HEIGHT = 1;
	static const float TILE_LENGTH = 1;

	static const float PLAYER_HEIGHT = 0.5;

	static const sf::Time PORTAL_COOLDOWN = sf::seconds(1);

	namespace Tile
	{
		struct tile_t
		{
			sf::Color c;
			int t;
		};

		const tile_t Floor        = {sf::Color(0xFF, 0xFF, 0xFF), 0x00};
		const tile_t Wall         = {sf::Color(0x00, 0x00, 0x00), 0x01};
		const tile_t UnderCeiling = {sf::Color(0x00, 0x00, 0x00), 0x12};
		const tile_t Bars         = {sf::Color(0x02, 0x00, 0x00), 0x03};


		const tile_t Spawn        = {sf::Color(0x15, 0x00, 0x00), 0x00};
	}

	namespace Special
	{
		struct special_t
		{
			sf::Color c;
			int t;
		};

		const special_t Portal = {sf::Color(0x00, 0x00, 0xFF), 0x20};
		const special_t Flag   = {sf::Color(0xFF, 0x00, 0x80), 0x21};
		const special_t Tree   = {sf::Color(0x00, 0xFF, 0x40), 0x22};

		const special_t Potion = {sf::Color(0x66, 0x33, 0xA0), 0x30};
		const special_t Key    = {sf::Color(0xFF, 0xFF, 0x10), 0x31};
		const special_t Door   = {sf::Color(0xFF, 0x77, 0x20), 0x04};
	}

	Level::Level()
		:m_seed(-1)

		,m_overWorld()
		,m_underWorld()
		,m_currentWorld(OverWorld)

		,m_spawn(0, PLAYER_HEIGHT, 0)

		,m_player()

		,m_mesh()

		,m_sprites()

		,m_portalTravelCooldown(sf::seconds(0))
		,m_portalPlaceCooldown(sf::seconds(0))
		,m_portalFlip(false)

		,m_totalTime(sf::seconds(0))

		,m_levelCompleted(false)
	{
	}

	Level::~Level()
	{
		m_sprites.clear();
		m_portals.clear();
		m_potions.clear();
	}

	bool Level::loadFromFile(const std::string& over, const std::string& under)
	{
		if (!m_overWorld.loadFromFile(over))
			return false;

		if (!m_underWorld.loadFromFile(under))
			return false;

		generateLevel();

		m_player.setPosition(m_spawn);
		m_player.update(sf::seconds(0), true);

		m_keysLeft = 0;
		m_portalsLeft = 0;

		return true;
	}

	void Level::update(sf::Time dt)
	{
		m_totalTime += dt;

	// Update Player
		m_player.update(dt, !m_portalFlip);

	// Check Player Collision
		glm::vec3 pa = checkCollision(m_player.getCollisionBox());
		for (auto& door : m_doors)
			pa += collisionAdjustment(door.getCollisionBox(), m_player.getCollisionBox());
		if (glm::length(pa) > 0)
			m_player.move(pa);

	// Update Portal
		updatePortal(dt);

	// Update Sprites
		for (auto& s : m_sprites)
		{
			s.update(dt);
			s.faceCamera(m_player.getPosition());
		}
		m_flag.faceCamera(m_player.getPosition());

	// Doors
		for (auto& d : m_doors)
		{
			d.update(dt);

			if (m_keysLeft > 0
			&& glm::length(m_player.getPosition() - d.getPosition()) < 1.0f
			&& sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
				if ((m_currentWorld == OverWorld && d.getPosition().y == 0)
				 || (m_currentWorld == UnderWorld && d.getPosition().y == -1))
				{
					d.open();
					Sound::Door.play();
					m_keysLeft--;
				}
			}
		}

	// Potions
		if (m_portalsLeft < 0)
			m_portalsLeft = 0;

		for (std::vector<Sprite>::iterator it = m_potions.begin(); it != m_potions.end(); )
		{
			Sprite& p = *it;
			p.faceCamera(m_player.getPosition());

			if (glm::length(p.getPosition()- m_player.getPosition()) < 0.9)
			{
				if (p.getScale().y > 0 && m_currentWorld == OverWorld)
				{
					m_portalsLeft++;
					it = m_potions.erase(it);
					Sound::Potion.play();
				}
				else if (p.getScale().y < 0 && m_currentWorld == UnderWorld)
				{
					m_portalsLeft++;
					it = m_potions.erase(it);
					Sound::Potion.play();
				}
				else
					it++;
			}
			else
				it++;
		}
	// Keys
		if (m_keysLeft < 0)
			m_keysLeft = 0;

		for (std::vector<Sprite>::iterator it = m_keys.begin(); it != m_keys.end(); )
		{
			Sprite& key = *it;
			key.faceCamera(m_player.getPosition());

			if (glm::length(key.getPosition()- m_player.getPosition()) < 0.9)
			{
				if (key.getScale().y > 0 && m_currentWorld == OverWorld)
				{
					m_keysLeft++;
					it = m_keys.erase(it);
					Sound::Key.play();
				}
				else if (key.getScale().y < 0 && m_currentWorld == UnderWorld)
				{
					m_keysLeft++;
					it = m_keys.erase(it);
					Sound::Key.play();
				}
				else
					it++;
			}
			else
				it++;
		}

	// Place Portal
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)
		 && m_portalsLeft > 0
		 && m_portalPlaceCooldown == sf::seconds(0))
		{
			bool placePortal = false;
			glm::vec3 p = m_player.getPosition();

			float i = p.x;
			float j = p.z;

			if (m_currentWorld == OverWorld)
			{
				if (getPixel(i, j, UnderWorld) != Tile::Wall.c)
					placePortal = true;
			}

			if (m_currentWorld == UnderWorld)
			{
				if (getPixel(i, j, OverWorld) != Tile::Wall.c)
					placePortal = true;
			}

			if (placePortal)
			{
				glm::vec3 f = m_player.getCamera()->forward();
				f.y = 0;
				f = glm::normalize(f);
				f.x *= TILE_WIDTH;
				f.z *= TILE_LENGTH;
				i += f.x - 0.5f;
				j += f.z - 0.5f;

				if (addPortal(i, j, m_currentWorld))
				{
					m_portalsLeft--;
					m_portalPlaceCooldown += PORTAL_COOLDOWN;
					Sound::PlacePortal.play();
				}
			}

		}

	// Complete Level?
		if (glm::length(m_flag.getPosition() - m_player.getPosition()) < 0.75f
		 && m_currentWorld == OverWorld
		 && !m_portalFlip)
			m_levelCompleted = true;
	}

	void Level::draw(gb::Program* shaderProgram)
	{
		shaderProgram->setUniform("time", m_totalTime.asSeconds());
		shaderProgram->setUniform("transform", glm::mat4());
		m_mesh.draw(shaderProgram);

		for (auto& s : m_sprites)
			s.draw(shaderProgram);

		for (auto& p : m_potions)
			p.draw(shaderProgram);

		for (auto& k : m_keys)
			k.draw(shaderProgram);

		for (auto& d : m_doors)
			d.draw(shaderProgram);

		m_flag.draw(shaderProgram);


		shaderProgram->setUniform("portal", true);
		for (auto& p : m_portals)
			p.draw(shaderProgram);
		shaderProgram->setUniform("portal", false);
	}

	Camera* Level::getPlayerCamera()
	{
		return m_player.getCamera();
	}

	void Level::generateLevel()
	{
		srand(m_seed);

		std::vector<Vertex> vertices;
		std::vector<int> indices;

	// OverWorld
		generateWorld(vertices, indices, OverWorld);
	// UnderWorld
		generateWorld(vertices, indices, UnderWorld);

		m_mesh.addVertices(vertices, indices);
	}

	void Level::generateWorld(std::vector<Vertex>& vertices, std::vector<int>& indices, WorldType w)
	{
		int textureOffset = w == UnderWorld ? 0x10 : 0x00;

		for (size_t i = 0; i < getWidth(); i++)
		{
			for (size_t j = 0; j < getHeight(); j++)
			{
				if (getPixel(i, j, w) == Tile::Wall.c)
					continue;

				if (getPixel(i, j, w).b > 0 && getPixel(i, j, w) != Tile::Floor.c)
					addSpecial(getPixel(i, j, w).b, i, j, w);

			// Generate floor
				if (w == UnderWorld)
					addFace(indices, vertices.size(), false);
				else
					addFace(indices, vertices.size(), true);
				addVertices(vertices, i, j, 0, FaceNormal::Y, Tile::Floor.t + textureOffset);

				addSpecial(0, i, j, w);

			// Set Spawn
				if (getPixel(i, j, w) == Tile::Spawn.c)
				{
					m_spawn = glm::vec3(TILE_WIDTH*(i+0.5f),
					                    PLAYER_HEIGHT,
					                    TILE_LENGTH*(j+0.5f));
				}

				if (w == UnderWorld)
				{
				// Generate ceiling
					if (w == OverWorld)
					{
						addFace(indices, vertices.size(), false);
						addVertices(vertices, i, j, 1, FaceNormal::Y, Tile::Floor.t + textureOffset);
					}
					else
					{
						addFace(indices, vertices.size(), true);
						addVertices(vertices, i, j, -1, FaceNormal::Y, Tile::UnderCeiling.t);
					}

				}

				int k = 0;
				if (w == UnderWorld)
					k = -1;

			// Add Bars
				if (getPixel(i, j, w) == Tile::Bars.c)
				{
					// from both sides
					addFace(indices, vertices.size(), false);
					addFace(indices, vertices.size(), true);

					if (getPixel(i, j-1, w) != Tile::Wall.c || getPixel(i, j+1, w) != Tile::Wall.c)
						addVertices(vertices, i, j+0.5f, k, FaceNormal::Z, Tile::Bars.t + textureOffset);
					else if (getPixel(i-1, j, w) != Tile::Wall.c || getPixel(i+1, j, w) != Tile::Wall.c)
						addVertices(vertices, i+0.5f, j, k, FaceNormal::X, Tile::Bars.t + textureOffset);

				}

				if (getPixel(i, j, w) == Special::Tree.c)
					continue;

			// Generate wall
				if (getPixel(i, j - 1, w) == Tile::Wall.c)
				{
					addFace(indices, vertices.size(), false);
					addVertices(vertices, i, j, k, FaceNormal::Z, Tile::Wall.t + textureOffset);
				}

				if (getPixel(i, j + 1, w) == Tile::Wall.c)
				{
					addFace(indices, vertices.size(), true);
					addVertices(vertices, i, j+1, k, FaceNormal::Z, Tile::Wall.t + textureOffset);
				}

				if (getPixel(i - 1, j, w) == Tile::Wall.c)
				{
					addFace(indices, vertices.size(), true);
					addVertices(vertices, i, j, k, FaceNormal::X, Tile::Wall.t + textureOffset);
				}

				if (getPixel(i + 1, j, w) == Tile::Wall.c)
				{
					addFace(indices, vertices.size(), false);
					addVertices(vertices, i+1, j, k, FaceNormal::X, Tile::Wall.t + textureOffset);
				}

			}
		}
	}

	void Level::addFace(std::vector<int>& indices, size_t startLocation, bool direction)
	{
		if (direction)
		{
			indices.push_back(startLocation + 0);
			indices.push_back(startLocation + 3);
			indices.push_back(startLocation + 2);
			indices.push_back(startLocation + 2);
			indices.push_back(startLocation + 1);
			indices.push_back(startLocation + 0);
		}
		else
		{
			indices.push_back(startLocation + 0);
			indices.push_back(startLocation + 1);
			indices.push_back(startLocation + 2);
			indices.push_back(startLocation + 2);
			indices.push_back(startLocation + 3);
			indices.push_back(startLocation + 0);
		}
	}

	void Level::addVertices(std::vector<Vertex>& vertices, float i, float j, float k, FaceNormal n, int tileType)
	{
		glm::vec4 uv = getTextureCoords(tileType);

		if (n == FaceNormal::Y)
		{
			vertices.push_back(Vertex(i * TILE_WIDTH, k*TILE_HEIGHT, j * TILE_LENGTH, uv[0], uv[1]));
			vertices.push_back(Vertex((i + 1) * TILE_WIDTH, k*TILE_HEIGHT, j * TILE_LENGTH, uv[2], uv[1]));
			vertices.push_back(Vertex((i + 1) * TILE_WIDTH, k*TILE_HEIGHT, (j + 1) * TILE_LENGTH, uv[2], uv[3]));
			vertices.push_back(Vertex(i * TILE_WIDTH, k*TILE_HEIGHT, (j + 1) * TILE_LENGTH, uv[0], uv[3]));
		}
		else if (n == FaceNormal::Z)
		{
			vertices.push_back(Vertex(i * TILE_WIDTH, k*TILE_HEIGHT, j * TILE_LENGTH, uv[0], uv[1]));
			vertices.push_back(Vertex((i + 1) * TILE_WIDTH, k*TILE_HEIGHT, j * TILE_LENGTH, uv[2], uv[1]));
			vertices.push_back(Vertex((i + 1) * TILE_WIDTH, (k+1)*TILE_HEIGHT, j * TILE_LENGTH, uv[2], uv[3]));
			vertices.push_back(Vertex(i * TILE_WIDTH, (k+1)*TILE_HEIGHT, j * TILE_LENGTH, uv[0], uv[3]));
		}
		else if (n == FaceNormal::X)
		{
			vertices.push_back(Vertex(i * TILE_WIDTH, k*TILE_HEIGHT, j * TILE_LENGTH, uv[0], uv[1]));
			vertices.push_back(Vertex(i * TILE_WIDTH, k*TILE_HEIGHT, (j + 1) * TILE_LENGTH, uv[2], uv[1]));
			vertices.push_back(Vertex(i * TILE_WIDTH, (k+1)*TILE_HEIGHT, (j + 1) * TILE_LENGTH, uv[2], uv[3]));
			vertices.push_back(Vertex(i * TILE_WIDTH, (k+1)*TILE_HEIGHT, j * TILE_LENGTH, uv[0], uv[3]));
		}
	}

	void Level::addFlora(float i, float j, WorldType w)
	{
		if (w == OverWorld)
		{
			Sprite grass(0x23 + rand() % 2);

			grass.setPosition(i + randomFloat(0, 1), 0, j + randomFloat(0, 1));

			grass.setRotation(45, 0, 1, 0);
			m_sprites.push_back(grass);

			grass.setPosition(i + randomFloat(0, 1), 0, j + randomFloat(0, 1));
			grass.setRotation(-45, 0, 1, 0);
			m_sprites.push_back(grass);
		}
		else
		{
			Sprite moss(0x33 + rand() % 2);

			moss.setPosition(i + randomFloat(0, 1), -1, j + randomFloat(0, 1));

			moss.setRotation(45, 0, 1, 0);
			m_sprites.push_back(moss);

			moss.setPosition(i + randomFloat(0, 1), -1, j + randomFloat(0, 1));
			moss.setRotation(-45, 0, 1, 0);
			m_sprites.push_back(moss);
		}
	}

	void Level::addSpecial(int blueValue, float i, float j, WorldType w)
	{
	// Grass
		if (blueValue == 0)
		{
			if (w == OverWorld)
			{
				for (int z = 0; z < 2; z++)
					addFlora(i, j, w);
			}
			else if (w == UnderWorld)
			{
				if (randomFloat(0, 10) > 5)
					addFlora(i, j, w);
			}
		}

	// Portal
		if (blueValue == Special::Portal.c.b)
			addPortal(i, j, w);

	// Flag
		if (blueValue == Special::Flag.c.b)
		{
			Sprite flag(Special::Flag.t);
			flag.setPosition((i+0.5)*TILE_WIDTH, 0, (j+0.5)*TILE_LENGTH);
			flag.setFaceCamera(true);
			flag.setOrigin(0.25, 0, 0);

			m_flag = flag;
		}
	// Potion
		if (blueValue == Special::Potion.c.b)
		{
			Sprite potion(Special::Potion.t);
			potion.setPosition((i+0.5)*TILE_WIDTH, 0, (j+0.5)*TILE_LENGTH);
			potion.setFaceCamera(true);

			if (w == UnderWorld)
				potion.setScale(1, -1, 1);

			m_potions.push_back(potion);
		}
	// Key
		if (blueValue == Special::Key.c.b)
		{
			Sprite key(Special::Key.t);
			key.setPosition((i+0.5)*TILE_WIDTH, 0, (j+0.5)*TILE_LENGTH);
			key.setFaceCamera(true);

			if (w == UnderWorld)
				key.setScale(1, -1, 1);

			m_keys.push_back(key);
		}
	// Tree
		if (blueValue == Special::Tree.c.b)
		{
			Sprite tree(Special::Tree.t);
			tree.setPosition((i+0.5)*TILE_WIDTH, 0, (j+0.5)*TILE_LENGTH);
			tree.setFaceCamera(true);

			tree.setScale(4.0f * glm::vec3(1, 1, 1));
			m_sprites.push_back(tree);
		}
	// Door
		if (blueValue == Special::Door.c.b)
		{
			bool iDoor = (getPixel(i, j - 1, w) == Tile::Wall.c) && (getPixel(i, j + 1, w) == Tile::Wall.c);
			bool jDoor = (getPixel(i - 1, j, w) == Tile::Wall.c) && (getPixel(i + 1, j, w) == Tile::Wall.c);

			if (iDoor ^ jDoor)
			{
				Door d;
				d.setPosition(i + 0.5, 0, j + 0.5);

				if (iDoor)
					d.setRotation(90, 0, 1, 0);

				if (w == UnderWorld)
					d.move(0, -1, 0);

				m_doors.push_back(d);
			}
		}
	}

	bool Level::addPortal(float i, float j, WorldType w)
	{
		Sprite portal(Special::Portal.t);
		portal.setPosition((i+0.5)*TILE_WIDTH, 0, (j+0.5)*TILE_LENGTH);
		portal.setFaceCamera(true);

		if (w == UnderWorld)
			portal.move(0, -1, 0);

		if ((getPixel(i, j-1, w) == Tile::Floor.c) || (getPixel(i, j+1, w) == Tile::Floor.c))
			portal.setRotation(90, 0, 1, 0);

		for (auto& p : m_portals)
		{
			if (p.getPosition() == portal.getPosition())
				return false;
		}

		m_portals.push_back(portal);

		return true;
	}

	glm::vec3 Level::collisionAdjustment(sf::FloatRect o1, sf::FloatRect o2) const
	{
		glm::vec2 depth;

		float left   = o1.left - (o2.left + o2.width);
		float right  = (o1.left + o1.width) - o2.left;
		float top    = o1.top - (o2.top + o2.height);
		float bottom = (o1.top + o1.height) - o2.top;

		if (left > 0 || right < 0)
			return glm::vec3();
		if (top > 0 || bottom < 0)
			return glm::vec3();

		if (fabs(left) < right)
			depth.x = left;
		else
			depth.x = right;

		if (fabs(top) < bottom)
			depth.y = top;
		else
			depth.y = bottom;

		if (fabs(depth.x) < fabs(depth.y))
			depth.y = 0;
		else
			depth.x = 0;


		return glm::vec3(depth.x, 0, depth.y);
	}


	glm::vec3 Level::checkCollision(const sf::FloatRect& box) const
	{
		glm::vec3 adjustment(0, 0, 0);

		int radius = 5;
		int startI = (box.left + 0.5f * box.width)/TILE_WIDTH  - radius;
		int startJ = (box.top + 0.5f * box.height)/TILE_HEIGHT - radius;
		int endI = startI + 2*radius;
		int endJ = startJ + 2*radius;

		if (startI < 0)
			startI = 0;
		if (startJ < 0)
			startJ = 0;

		if (endI > getWidth())
			endI = getWidth();
		if (endJ > getHeight())
			endJ = getHeight();

		for (size_t i = startI; i < endI; i++)
		{
			for (size_t j = startJ; j < endJ; j++)
			{
				if (getPixel(i, j, m_currentWorld) == Tile::Wall.c
				 || getPixel(i, j, m_currentWorld) == Tile::Bars.c)
				{
					sf::FloatRect wall;
					wall.left   = i * TILE_WIDTH;
					wall.top    = j * TILE_LENGTH;
					wall.width  = TILE_WIDTH;
					wall.height = TILE_HEIGHT;

					if (wall.intersects(box))
						adjustment += collisionAdjustment(wall, box);
				}
			}
		}


		return adjustment;
	}

	void Level::updatePortal(sf::Time dt)
	{
		m_portalTravelCooldown -= dt;
		m_portalPlaceCooldown  -= dt;

		if (m_portalTravelCooldown < sf::seconds(0))
		{
			m_portalTravelCooldown = sf::seconds(0);
			m_portalFlip = false;
		}

		if (m_portalPlaceCooldown < sf::seconds(0))
			m_portalPlaceCooldown = sf::seconds(0);


		if (m_portalTravelCooldown > sf::seconds(0) && !m_portalFlip)
		{
			if (m_currentWorld == OverWorld)
			{
				m_currentWorld = UnderWorld;
				m_portalFlip = true;
			}
			else
			{
				m_currentWorld = OverWorld;
				m_portalFlip = true;
			}

		}

		if (m_portalFlip)
		{
			Camera& c = *m_player.getCamera();

			float a = 180;
			float s = 1;
			if (m_currentWorld == UnderWorld)
			{
				a = 0;
				s = -1;
			}
			float r = (PORTAL_COOLDOWN - m_portalTravelCooldown).asSeconds();
			r /= PORTAL_COOLDOWN.asSeconds();


			c.setRollAngle(a + 180*r);
			m_player.setPosition(m_player.getPosition().x, s*(r-0.5f), m_player.getPosition().z);
		}
		else
		{
			if (m_currentWorld == UnderWorld)
				m_player.getCamera()->setRollAngle(180);
			else
				m_player.getCamera()->setRollAngle(0);
		}

	// Loop through all portals
		for (auto& p : m_portals)
		{
			p.update(dt);
			p.faceCamera(m_player.getPosition());

			if (m_portalTravelCooldown.asMicroseconds() <= 0)
			{
				if ((p.getPosition().y < 0 && m_currentWorld != UnderWorld)
				 || (p.getPosition().y > 0 && m_currentWorld != OverWorld))
					continue;

				glm::vec3 l = p.getPosition() - m_player.getPosition();
				float l2 = l.x*l.x + l.z*l.z;

				// Only flip if close enough and on the correct side
				if (0.8f > l2
				 && ((p.getPosition().y == -1 && m_currentWorld == UnderWorld) || (p.getPosition().y == 0  && m_currentWorld == OverWorld)))
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
					{
						m_portalTravelCooldown += PORTAL_COOLDOWN;
						Sound::Portal.play();
					}
				}
			}
		}
	}

	unsigned int Level::getWidth() const
	{
		return m_overWorld.getSize().x;
	}

	unsigned int Level::getHeight() const
	{
		return m_overWorld.getSize().y;
	}

	const Mesh& Level::getMesh() const
	{
		return m_mesh;
	}

	const Level::WorldType& Level::getCurrentWorldType() const
	{
		return m_currentWorld;
	}

	Level& Level::setPixel(unsigned int x, unsigned int z, WorldType w, const sf::Color& color)
	{
		if (w == OverWorld)
			m_overWorld.setPixel(x, z, color);
		else
			m_underWorld.setPixel(x, z, color);
		return *this;
	}

	sf::Color Level::getPixel(unsigned int x, unsigned int z, WorldType w) const
	{
		if (w == OverWorld)
			return m_overWorld.getPixel(x, z);
		else
			return m_underWorld.getPixel(x, z);
	}

	glm::vec3 Level::getTileSize()
	{
		return glm::vec3(TILE_WIDTH, TILE_HEIGHT, TILE_LENGTH);
	}

	bool Level::levelCompleted() const
	{
		return m_levelCompleted;
	}

	unsigned int Level::getNumberOfPortalsLeft() const
	{
		return m_portalsLeft;
	}

	unsigned int Level::getNumberOfKeys() const
	{
		return m_keysLeft;
	}

}


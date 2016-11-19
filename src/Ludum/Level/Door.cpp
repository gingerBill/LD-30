#include <Ludum/Level/Door.hpp>

#include <Ludum/Common.hpp>

namespace gb
{
	static const float DOOR_DEPTH = 0.25;
	static const sf::Time DOOR_TIME = sf::seconds(0.9);

	Door::Door()
		:Sprite(0x04)
		,m_state(Closed)
		,m_previousState(Open)
		,m_movingTime(sf::seconds(0))
		,m_waitTime(sf::seconds(0))
		,m_toggle(false)
	{
	}

	void Door::update(sf::Time dt)
	{
		Sprite::update(dt);

		m_waitTime -= dt;
		if (m_waitTime.asSeconds() > 0 && !m_toggle)
			return;

		if (m_previousState == Open
		 && m_state == Closed
		 && m_movingTime < DOOR_TIME
		 && m_toggle)
		{
			m_movingTime += dt;
			if (getRotation().angle == 90)
				move(0, 0, dt.asSeconds());
			else
				move(dt.asSeconds(), 0, 0);
		}
		else if (m_movingTime >= DOOR_TIME && m_toggle)
		{
			m_state = Open;
			m_previousState = Closed;
			m_toggle = false;
			m_movingTime = sf::seconds(0);
			m_waitTime = 2.0f * DOOR_TIME;
		}

		if (m_previousState == Closed
		 && m_state == Open
		 && m_movingTime < DOOR_TIME
		 && m_toggle)
		{
			m_movingTime += dt;
			if (getRotation().angle == 90)
				move(0, 0, -dt.asSeconds());
			else
				move(-dt.asSeconds(), 0, 0);
		}
		else if (m_movingTime >= DOOR_TIME && m_toggle)
		{
			m_state = Closed;
			m_previousState = Open;
			m_toggle = false;
			m_movingTime = sf::seconds(0);
			m_waitTime = 2.0f * DOOR_TIME;
		}
	}

	void Door::toggle()
	{
		if (m_state == Moving || m_toggle)
			return;

		m_previousState = m_state;

		if (m_state == Open)
			m_state = Closed;
		else if (m_state == Closed)
			m_state = Open;

		m_toggle = true;
		m_waitTime = 1.1f * DOOR_TIME;
	}

	void Door::open()
	{
		if (m_state == Moving || m_state == Open)
			return;

		toggle();
	}


	void Door::generateMesh(int tileType)
	{
		glm::vec4 uv = getTextureCoords(tileType);

		std::vector<Vertex> vertices;
		std::vector<int> indices;

		// Front
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(0);

		indices.push_back(4);
		indices.push_back(7);
		indices.push_back(6);
		indices.push_back(6);
		indices.push_back(5);
		indices.push_back(4);

		vertices.push_back(Vertex(0, 0, 0, uv[0], uv[1]));
		vertices.push_back(Vertex(1, 0, 0, uv[2], uv[1]));
		vertices.push_back(Vertex(1, 1, 0, uv[2], uv[3]));
		vertices.push_back(Vertex(0, 1, 0, uv[0], uv[3]));

		vertices.push_back(Vertex(0, 1, 0, uv[0], uv[1]));
		vertices.push_back(Vertex(1, 1, 0, uv[2], uv[1]));
		vertices.push_back(Vertex(1, 2, 0, uv[2], uv[3]));
		vertices.push_back(Vertex(0, 2, 0, uv[0], uv[3]));

		// Back
		indices.push_back(0 + 8);
		indices.push_back(1 + 8);
		indices.push_back(2 + 8);
		indices.push_back(2 + 8);
		indices.push_back(3 + 8);
		indices.push_back(0 + 8);

		indices.push_back(4 + 8);
		indices.push_back(5 + 8);
		indices.push_back(6 + 8);
		indices.push_back(6 + 8);
		indices.push_back(7 + 8);
		indices.push_back(4 + 8);

		vertices.push_back(Vertex(0, 0, DOOR_DEPTH, uv[0], uv[1]));
		vertices.push_back(Vertex(1, 0, DOOR_DEPTH, uv[2], uv[1]));
		vertices.push_back(Vertex(1, 1, DOOR_DEPTH, uv[2], uv[3]));
		vertices.push_back(Vertex(0, 1, DOOR_DEPTH, uv[0], uv[3]));

		vertices.push_back(Vertex(0, 1, DOOR_DEPTH, uv[0], uv[1]));
		vertices.push_back(Vertex(1, 1, DOOR_DEPTH, uv[2], uv[1]));
		vertices.push_back(Vertex(1, 2, DOOR_DEPTH, uv[2], uv[3]));
		vertices.push_back(Vertex(0, 2, DOOR_DEPTH, uv[0], uv[3]));

		// side 1
		indices.push_back(0 + 16);
		indices.push_back(1 + 16);
		indices.push_back(2 + 16);
		indices.push_back(2 + 16);
		indices.push_back(3 + 16);
		indices.push_back(0 + 16);

		vertices.push_back(Vertex(0, 0, 0,          uv[0], uv[1]));
		vertices.push_back(Vertex(0, 0, DOOR_DEPTH, uv[2], uv[1]));
		vertices.push_back(Vertex(0, 2, DOOR_DEPTH, uv[2], uv[3]));
		vertices.push_back(Vertex(0, 2, 0,          uv[0], uv[3]));

		// side 2
		indices.push_back(4 + 16);
		indices.push_back(7 + 16);
		indices.push_back(6 + 16);
		indices.push_back(6 + 16);
		indices.push_back(5 + 16);
		indices.push_back(4 + 16);

		vertices.push_back(Vertex(1, 0, 0,          uv[0], uv[1]));
		vertices.push_back(Vertex(1, 0, DOOR_DEPTH, uv[2], uv[1]));
		vertices.push_back(Vertex(1, 2, DOOR_DEPTH, uv[2], uv[3]));
		vertices.push_back(Vertex(1, 2, 0,          uv[0], uv[3]));

		m_mesh.addVertices(vertices, indices);

		setOrigin(0.5, 0, DOOR_DEPTH/2.0f);
	}

	sf::FloatRect Door::getCollisionBox() const
	{
		sf::FloatRect cb;

		cb.left = getPosition().x - getOrigin().x;
		cb.width = 1;
		cb.top = getPosition().z  - getOrigin().z;
		cb.height = DOOR_DEPTH;

		return cb;
	}

}

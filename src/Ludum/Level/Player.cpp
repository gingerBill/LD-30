#include <Ludum/Level/Player.hpp>

#include <SFML/Window/Keyboard.hpp>

namespace gb
{
	Player::Player()
		:Transformable()
		,m_camera()
		,m_maxHealth(100)
		,m_health(m_maxHealth)
		,m_collisionBox()
	{
		setPosition(0, 0.5, 0);
	}

	void Player::update(sf::Time dt, bool controls)
	{
		if (controls)
			updatePosition(dt);

		m_camera.setPosition(getPosition());
	}

	void Player::updatePosition(sf::Time dt)
	{
		float speed = 5.0f;
		glm::vec3 movementVector(0, 0, 0);

		glm::vec3 forward = m_camera.forward();
		forward.y = 0;
		forward = glm::normalize(forward);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			movementVector += forward;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			movementVector -= forward;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			movementVector += m_camera.left();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			movementVector += m_camera.right();

		if (glm::length(movementVector) > 0)
			move(movementVector * speed * dt.asSeconds());
	}

	Camera* Player::getCamera()
	{
		return &m_camera;
	}


	int Player::getHealth() const
	{
		return m_health;
	}

	void Player::damage(int h)
	{
		if (h < 0)
			return;

		m_health -= h;

		if (m_health < 0)
			m_health = 0;

	}

	void Player::heal(int h)
	{
		if (h < 0)
			return;

		m_health += h;

		if (m_health > m_maxHealth)
			m_health = m_maxHealth;
	}

	sf::FloatRect Player::getCollisionBox() const
	{
		sf::FloatRect cb;

		float playerWidth = 0.5f;

		cb.width = playerWidth;
		cb.height = playerWidth;

		cb.left = getPosition().x - 0.5f*playerWidth;
		cb.top  = getPosition().z - 0.5f*playerWidth;

		return cb;
	}
}

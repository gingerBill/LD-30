#ifndef Ludum_Level_Player_hpp
#define Ludum_Level_Player_hpp

#include <Ludum/Graphics/Transformable.hpp>
#include <Ludum/Graphics/Camera.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace gb
{
	class Player : public Transformable
	{
	public:
		Player();

		void update(sf::Time dt, bool controls);

		Camera* getCamera();

		int getHealth() const;
		void damage(int h);
		void heal(int h);

		sf::FloatRect getCollisionBox() const;

	private:
		void updatePosition(sf::Time dt);

		Camera m_camera;

		int m_maxHealth;
		int m_health;

		sf::FloatRect m_collisionBox;

	};
}

#endif

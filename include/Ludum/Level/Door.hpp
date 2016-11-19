#ifndef Ludum_Level_Door_hpp
#define Ludum_Level_Door_hpp

#include <Ludum/Level/Sprite.hpp>

namespace gb
{
	class Door : public Sprite
	{
	public:
		Door();

		void update(sf::Time dt);

		void toggle();
		void open();

		sf::FloatRect getCollisionBox() const;

	private:
		enum State
		{
			Open,
			Moving,
			Closed,
		};

		void generateMesh(int tileType);

		State m_state;
		State m_previousState;
		sf::Time m_movingTime;
		sf::Time m_waitTime;

		bool m_toggle;
	};
}

#endif

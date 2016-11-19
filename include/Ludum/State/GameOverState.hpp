#ifndef Ludum_State_GameOverState_hpp
#define Ludum_State_GameOverState_hpp

#include <Ludum/State/State.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace gb
{
	class GameOverState : public State
	{
	public:
		GameOverState(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		sf::Text   m_pausedText;
	};
}

#endif

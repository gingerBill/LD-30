#ifndef Ludum_State_PauseState_hpp
#define Ludum_State_PauseState_hpp

#include <Ludum/State/State.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace gb
{
	class PauseState : public State
	{
	public:
		PauseState(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		sf::Text   m_pausedText;
	};
}

#endif

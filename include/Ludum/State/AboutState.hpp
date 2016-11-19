#ifndef Ludum_State_AboutState_hpp
#define Ludum_State_AboutState_hpp

#include <Ludum/State/State.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace gb
{
	class AboutState : public State
	{
	public:
		AboutState(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		sf::Text m_aboutText;
	};
}

#endif

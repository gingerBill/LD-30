#ifndef Ludum_State_MenuState_hpp
#define Ludum_State_MenuState_hpp

#include <Ludum/State/State.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>

namespace gb
{
	class MenuState : public State
	{
	public:
		MenuState(StateStack& stack, Context context);

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		int m_button;
		int m_maxButton;

		sf::Texture m_backgroundTexture;
		sf::Sprite m_backgroundSprite;
		sf::Text m_text;

		std::vector<std::string> m_buttons;
	};
}

#endif

#include <Ludum/State/AboutState.hpp>

#include <Ludum/Common.hpp>
#include <Ludum/Sound.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

namespace gb
{
	AboutState::AboutState(StateStack& stack, Context context)
		:State(stack, context)
		,m_aboutText()
	{
		sf::Font& font = *context.font;
		sf::Vector2f windowSize(context.window->getSize());

		m_aboutText.setFont(font);
		m_aboutText.setString("About\n"
		                      "\n"
		                      "The Flipside was created by gingerBill for the Ludum Dare 48 hour challenge in\n"
		                      "August 2014; the theme was `Connected Worlds`.\n"
		                      "The aim of the game is to travel through and place portals which will flip the\n"
		                      "player to the other side of the map in order to collect portal potions to create\n"
		                      "portals and keys to unlock doors, to complete the level's end which is marked by\n"
		                      "a red flag.\n"
		                      "\n"
		                      "\n"
		                      "Controls\n"
		                      "\n"
		                      "[W] Forward\n"
		                      "[A] Left\n"
		                      "[S] Back\n"
		                      "[D] Right\n"
		                      "[E] Use Portal or Use Door (with key)\n"
		                      "[Space] Place Portal\n"
		                      "[Esc] Pause\n"
		                      "\n"
		                      "> Return to Menu"
		                       );


		m_aboutText.setCharacterSize(16);
		m_aboutText.setOrigin(m_aboutText.getLocalBounds().width/2,
		                       m_aboutText.getLocalBounds().height/2);
		m_aboutText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

		getContext().window->setMouseCursorVisible(true);
	}

	void AboutState::draw()
	{
		sf::RenderWindow& window = *getContext().window;

		window.pushGLStates();
		{
			window.setView(window.getDefaultView());

			sf::RectangleShape backgroundShape;
			backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
			backgroundShape.setSize(window.getView().getSize());

			window.draw(backgroundShape);
			window.draw(m_aboutText);
		}
		window.popGLStates();
	}

	bool AboutState::update(sf::Time)
	{
		sf::RenderWindow& window = *getContext().window;
		window.setMouseCursorVisible(true);
		return false;
	}

	bool AboutState::handleEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{

			if (event.key.code == sf::Keyboard::Return
			 || event.key.code == sf::Keyboard::Space)
			{
				requestStackPop();
			}
		}
		return false;
	}
}

#include <Ludum/State/GameOverState.hpp>

#include <Ludum/Common.hpp>
#include <Ludum/Sound.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

namespace gb
{
	GameOverState::GameOverState(StateStack& stack, Context context)
		:State(stack, context)
		,m_pausedText()
	{
		sf::Font& font = *context.font;
		sf::Vector2f windowSize(context.window->getSize());


		m_pausedText.setFont(font);
		m_pausedText.setString("You have completed The Flipside!\n"
		                       "\n"
		                       "If you enjoyed this game, please\n"
		                       "share it so that others can enjoy\n"
		                       "       The Flipside as well.\n"
		                       "\n"
		                       "     [Return] Return to Menu\n"
		                       );
		m_pausedText.setCharacterSize(32);
		m_pausedText.setOrigin(m_pausedText.getLocalBounds().width/2,
		                       m_pausedText.getLocalBounds().height/2);
		m_pausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

		getContext().window->setMouseCursorVisible(true);
	}

	void GameOverState::draw()
	{
		sf::RenderWindow& window = *getContext().window;

		window.pushGLStates();
		{
			window.setView(window.getDefaultView());

			sf::RectangleShape backgroundShape;
			backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
			backgroundShape.setSize(window.getView().getSize());

			window.draw(backgroundShape);
			window.draw(m_pausedText);
		}
		window.popGLStates();
	}

	bool GameOverState::update(sf::Time)
	{
		sf::RenderWindow& window = *getContext().window;
		window.setMouseCursorVisible(true);
		return false;
	}

	bool GameOverState::handleEvent(const sf::Event& event)
	{

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
		{
			requestStackPop();
			requestStackPush(States::Menu);
		}


		return false;
	}
}

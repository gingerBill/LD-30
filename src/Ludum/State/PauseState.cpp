#include <Ludum/State/PauseState.hpp>

#include <Ludum/Common.hpp>
#include <Ludum/Sound.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

namespace gb
{
	PauseState::PauseState(StateStack& stack, Context context)
		:State(stack, context)
		,m_pausedText()
	{
		sf::Font& font = *context.font;
		sf::Vector2f windowSize(context.window->getSize());

		m_pausedText.setFont(font);
		m_pausedText.setString("    Game Paused\n"
		                       "\n"
		                       "   [Esc] Continue\n"
		                       "[Return] Menu\n"
		                       ""
		                       );


		m_pausedText.setCharacterSize(32);
		m_pausedText.setOrigin(m_pausedText.getLocalBounds().width/2,
		                       m_pausedText.getLocalBounds().height/2);
		m_pausedText.setPosition(0.5f * windowSize.x, 0.5f * windowSize.y);

		getContext().window->setMouseCursorVisible(true);
	}

	void PauseState::draw()
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

	bool PauseState::update(sf::Time)
	{
		sf::RenderWindow& window = *getContext().window;
		window.setMouseCursorVisible(true);
		return false;
	}

	bool PauseState::handleEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			sf::RenderWindow& window = *getContext().window;

			window.setMouseCursorVisible(false);
			sf::Mouse::setPosition(sf::Vector2i(window.getSize())/2, window);

			requestStackPop();
		}

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
		{
			requestStateClear();
			requestStackPush(States::Menu);
		}

		return false;
	}
}

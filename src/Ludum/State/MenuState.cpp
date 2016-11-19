#include <Ludum/State/MenuState.hpp>

#include <Ludum/Common.hpp>
#include <Ludum/Sound.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

namespace gb
{
	MenuState::MenuState(StateStack& stack, Context context)
		:State(stack, context)
		,m_button(0)
		,m_maxButton(3)
	{
		sf::Font& font = *context.font;
		sf::Vector2f windowSize(context.window->getSize());

		if (!m_backgroundTexture.loadFromFile(resourcePath() + "Data/Texture/menuBackground.png"))
			throw std::runtime_error("could not load menuBackground\n");

		m_backgroundSprite.setTexture(m_backgroundTexture);
		m_backgroundSprite.setOrigin(m_backgroundSprite.getLocalBounds().width/2,
		                            m_backgroundSprite.getLocalBounds().height/2);

		m_backgroundSprite.setPosition(0.5f*sf::Vector2f(windowSize));

		m_buttons.push_back("Play");
		m_buttons.push_back("About");
		m_buttons.push_back("Quit");

		m_text.setFont(font);


		m_text.setCharacterSize(32);
		m_text.setPosition(windowSize.x - 200, windowSize.y - 200);

		getContext().window->setMouseCursorVisible(true);
	}

	void MenuState::draw()
	{
		sf::RenderWindow& window = *getContext().window;

		window.pushGLStates();
		{
			window.setView(window.getDefaultView());

			window.draw(m_backgroundSprite);
			window.draw(m_text);
		}
		window.popGLStates();
	}

	bool MenuState::update(sf::Time)
	{
		sf::RenderWindow& window = *getContext().window;
		window.setMouseCursorVisible(true);

		if (m_button < 0)
			m_button = m_maxButton - 1;
		if (m_button == m_maxButton)
			m_button = 0;

		std::string textStr;
		for (size_t i = 0; i < m_maxButton; i++)
		{
			if (i == m_button)
				textStr += "> ";
			textStr += m_buttons[i] + '\n';
		}

		m_text.setString(textStr);

		return false;
	}

	bool MenuState::handleEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Down
			 || event.key.code == sf::Keyboard::Right
			 || event.key.code == sf::Keyboard::S
			 || event.key.code == sf::Keyboard::D)
				m_button++;

			if (event.key.code == sf::Keyboard::Up
			 || event.key.code == sf::Keyboard::Left
			 || event.key.code == sf::Keyboard::W
			 || event.key.code == sf::Keyboard::A)
				m_button--;

			if (event.key.code == sf::Keyboard::Return
			 || event.key.code == sf::Keyboard::Space
			 || event.key.code == sf::Keyboard::E)
			{
				if (m_button == 0)
				{
					requestStackPop();
					requestStackPush(States::Game);
				}
				else if (m_button == 1)
				{
					requestStackPush(States::About);
				}
				else if (m_button == 2)
				{
					requestStateClear();
				}
			}
		}



		return false;
	}
}

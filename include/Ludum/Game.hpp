#ifndef Ludum_Game_hpp
#define Ludum_Game_hpp

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

#include <Ludum/State/StateStack.hpp>

namespace gb
{
	class Game
	{
	public:
		Game();

		void run();

	private:
		void init();

		void handleEvents();
		void update(sf::Time time);
		void render();

		void registerStates();

		void clearWindow(const sf::Color& color);

		bool m_hasFocus;

		sf::RenderWindow m_window;
		sf::Texture m_texture;

		unsigned int m_levelNumber;

		sf::Font m_font;


		StateStack m_stateStack;
	};
}

#endif

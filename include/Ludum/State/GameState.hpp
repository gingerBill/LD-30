#ifndef Ludum_State_GameState_hpp
#define Ludum_State_GameState_hpp

#include <Ludum/State/State.hpp>
#include <Ludum/Level/Level.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Music.hpp>

namespace gb
{
	class GameState : public State
	{
	public:
		GameState(StateStack& stack, Context context);

		~GameState();

		virtual void draw();
		virtual bool update(sf::Time dt);
		virtual bool handleEvent(const sf::Event& event);

	private:
		void loadLevel();

		void drawUI();

		sf::RenderWindow& m_window;

		sf::Music m_overMusic;
		sf::Music m_underMusic;

		Program* m_shaderProgram;
		Material m_material;
		Level m_level;

		unsigned int m_levelNumber;

	};
}

#endif
